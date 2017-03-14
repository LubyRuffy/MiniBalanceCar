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

/***************************************************************
* Function name:  MPU6050_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void MPU6050_Init(void)
{
    int i=0,j=0;
    //在初始化之前需要延时一段时间
    for (i=0; i<1000; i++)
    {
        for (j=0; j<1000; j++)
        {
            ;
        }
    }
    
    IIC_Write(PWR_MGMT_1, 0x00);        //Internal 8MHz oscillator
    IIC_Write(SMPLRT_DIV, 0x07);        //Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    IIC_Write(CONFIG , 0x06);	        //Input disabled, Accelerometer Bandwidth 5K Delay 19ms, Gyroscope Bandwidth 5K Delay 18.6ms Fs 1K
    IIC_Write(ACCEL_CONFIG , 0x01);	    //+/_2g 
    IIC_Write(GYRO_CONFIG, 0x18);       //+/_2000deg/s)
}

/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    IIC_Read_Buffer(ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    IIC_Read_Buffer(GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
    u8 buf[2];
    IIC_Read_Buffer(TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(short*Temperature)
{
    short temp3;
    u8 buf[2];

    IIC_Read_Buffer(TEMP_OUT_H,buf,2);     //读取温度值
    temp3= (buf[0] << 8) | buf[1];
    *Temperature=(((double) (temp3 + 13200)) / 280)-13;
}
/*********************************************END OF FILE**********************/
