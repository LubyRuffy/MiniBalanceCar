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

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (100)

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
    int result=0;
    
	I2C_GPIO_Config();
    I2C_Mode_Config();
    
	result = mpu_init();
	if(!result)
    {
        printf("mpu initialization complete......\r\n");		//mpu initialization complete	 	  

		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))		//mpu_set_sensor
		{
            printf("mpu_set_sensor complete ......\r\n");
        }
		else
        {
			printf("mpu_set_sensor come across error ......\r\n");
            result = 1;
        }
        
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))	//mpu_configure_fifo
		{
            printf("mpu_configure_fifo complete ......\r\n");
        }
        else
        {
			printf("mpu_configure_fifo come across error ......\r\n");
            result = 1;
        }
        
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))	   	  		//mpu_set_sample_rate
        {		
            printf("mpu_set_sample_rate complete ......\r\n");
        }
		else
        {
		 	printf("mpu_set_sample_rate error ......\r\n");
            result = 1;
        }

		if(!dmp_load_motion_driver_firmware())   	  			//dmp_load_motion_driver_firmvare
        {
			printf("dmp_load_motion_driver_firmware complete ......\r\n");
        }
		else
        {
			printf("dmp_load_motion_driver_firmware come across error ......\r\n");
            result = 1;
        }

		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) 	  //dmp_set_orientation
		{
            printf("dmp_set_orientation complete ......\r\n");
        }
		else
        {
		 	printf("dmp_set_orientation come across error ......\r\n");
            result = 1;
        }

		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		    DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		    DMP_FEATURE_GYRO_CAL))		   	 					 //dmp_enable_feature
        {
		 	printf("dmp_enable_feature complete ......\r\n");
        }
		else
        {
		 	printf("dmp_enable_feature come across error ......\r\n");
            result = 1;
        }

		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))   	 			 //dmp_set_fifo_rate
        {
		 	printf("dmp_set_fifo_rate complete ......\r\n");
        }
		else
        {
		 	printf("dmp_set_fifo_rate come across error ......\r\n");
            result = 1;
        }

		run_self_test();

		if(!mpu_set_dmp_state(1))
        {
		 	PrintChar("mpu_set_dmp_state complete ......\r\n");
        }
		else
        {
		 	PrintChar("mpu_set_dmp_state come across error ......\r\n");
            result = 1;
        }
    }
    return 0;
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
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors & INV_WXYZ_QUAT )
	{
		q0 = quat[0] / q30;	
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;

		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;	// pitch
		Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3;	// roll
		Yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	    //printf("the Pitch is   : %d\r\n",(int)Pitch);
	}
}

/*********************************************END OF FILE**********************/
