/**
******************************************************************************
* @file    MPU6050.c
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar MPU6050 driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "MPU6050.h"
#include "I2C.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "USART1.h"
#include "math.h"
#include "string.h"

/* Data requested by client. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)

#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (100)

struct rx_s {
    unsigned char header[3];
    unsigned char cmd;
};
struct hal_s {
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned short report;
    unsigned short dmp_features;
    unsigned char motion_int_mode;
    struct rx_s rx;
};
static struct hal_s hal = {0};


static signed char gyro_orientation[9] = {-1, 0, 0,
                                           0,-1, 0,
                                           0, 0, 1};
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
float Pitch,Roll,Yaw;
unsigned long sensor_timestamp;
short gyro[3], accel[3], sensors;
unsigned char more;
long quat[4];
                                           
/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
static inline unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
} 
                                           
static inline unsigned short inv_orientation_matrix_to_scalar(
    const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}
  
static inline void run_self_test(void)
{
    int result;
    char test_packet[4] = {0};
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        printf("setting bias succesfully ......\r\n");
    }
    else
    {
        printf("bias has not been modified ......\r\n");
    }
    /* Report results. */
    test_packet[0] = 't';
    test_packet[1] = result;
    
    printf("Result mask: %c\r\n", test_packet[1]+'0');
}

/***************************************************************
* Function name:  MPU6050_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
int MPU6050_Init(void)
{
    int result;
    unsigned char accel_fsr;
    unsigned short gyro_rate, gyro_fsr;
    
    I2C_GPIO_Config();
    I2C_Mode_Config();
    
    result = mpu_init();
    if(!result)
    {
        printf("mpu initialization complete......\r\n");        //mpu initialization complete         

        /* Get/set hardware configuration. Start gyro. */
        /* Wake up all sensors. */
        mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        /* Push both gyro and accel data into the FIFO. */
        mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
        mpu_set_sample_rate(DEFAULT_MPU_HZ);
        /* Read back configuration in case it was set improperly. */
        mpu_get_sample_rate(&gyro_rate);
        mpu_get_gyro_fsr(&gyro_fsr);
        mpu_get_accel_fsr(&accel_fsr);

        /* Initialize HAL state variables. */
        memset(&hal, 0, sizeof(hal));
        hal.sensors = ACCEL_ON | GYRO_ON;
        hal.report = PRINT_QUAT;
        
        /* To initialize the DMP:
         * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
         *    inv_mpu_dmp_motion_driver.h into the MPU memory.
         * 2. Push the gyro and accel orientation matrix to the DMP.
         * 3. Register gesture callbacks. Don't worry, these callbacks won't be
         *    executed unless the corresponding feature is enabled.
         * 4. Call dmp_enable_feature(mask) to enable different features.
         * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
         * 6. Call any feature-specific control functions.
         *
         * To enable the DMP, just call mpu_set_dmp_state(1). This function can
         * be called repeatedly to enable and disable the DMP at runtime.
         *
         * The following is a short summary of the features supported in the DMP
         * image provided in inv_mpu_dmp_motion_driver.c:
         * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
         * 200Hz. Integrating the gyro data at higher rates reduces numerical
         * errors (compared to integration on the MCU at a lower sampling rate).
         * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
         * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
         * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
         * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
         * an event at the four orientations where the screen should rotate.
         * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
         * no motion.
         * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
         * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
         * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
         * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
         */
        dmp_load_motion_driver_firmware();
        dmp_set_orientation(
        inv_orientation_matrix_to_scalar(gyro_orientation));
//        dmp_register_tap_cb(tap_cb);
//        dmp_register_android_orient_cb(android_orient_cb);
        hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
        dmp_enable_feature(hal.dmp_features);
        dmp_set_fifo_rate(DEFAULT_MPU_HZ);
        mpu_set_dmp_state(1);
        hal.dmp_on = 1;

        run_self_test();
    }
    return result;
}

/***************************************************************
* Function name:  MPU6050_Refresh_Pose()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void MPU6050_Refresh_Pose(void)
{
    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);
    if(sensors & INV_WXYZ_QUAT )
    {
        q0 = quat[0] / q30;
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;

        Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;  // pitch
        //Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;  // roll
        //Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;    //yaw
    }
}

/*********************************************END OF FILE**********************/
