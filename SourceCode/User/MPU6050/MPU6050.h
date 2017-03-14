/**
******************************************************************************
* @file    MPU6050.h
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar MPU6050 driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __MPU6050_H__
#define	__MPU6050_H__
#include "stm32f10x.h"

#define WHO_AM_I            0x75
#define PWR_MGMT_1          0x6B
#define SMPLRT_DIV          0x19
#define CONFIG              0x1A
#define ACCEL_CONFIG        0x1C
#define GYRO_CONFIG         0x1B
#define GYRO_OUT            0x43
#define ACC_OUT             0x3B
#define TEMP_OUT_H          0x41

/***************************************************************
* Function name:  MPU6050_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void MPU6050_Init(void);

void MPU6050ReadTemp(short *tempData);
void MPU6050ReadGyro(short *gyroData);
void MPU6050ReadAcc(short *accData);
void MPU6050_ReturnTemp(short*Temperature);

#endif
/*********************************************END OF FILE**********************/
