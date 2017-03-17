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

#define q30  1073741824.0f

extern float Pitch,Roll,Yaw;
extern short gyro[3], accel[3];

/***************************************************************
* Function name:  MPU6050_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
int MPU6050_Init(void);

/***************************************************************
* Function name:  MPU6050_Refresh_Pose()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void MPU6050_Refresh_Pose(void);

#endif
/*********************************************END OF FILE**********************/
