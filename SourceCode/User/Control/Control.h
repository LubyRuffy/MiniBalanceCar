/**
******************************************************************************
* @file    Control.h
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar minibalance_control driver define
* @history    
*
* Copyright (c) 2016, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"

/***********************??????**************************************
*************************************************************************/
//------------------------------------------------------------------------------
#define CONTROL_PERIOD                  5       // Every 5 ms adjust all control


#define ENCODE_CONSTANT                 390

#define SPEED_CONTROL_COUNT             20      // 20 ms        ??20?

#define SPEED_CONTROL_PERIOD            (SPEED_CONTROL_COUNT * CONTROL_PERIOD) //unit: ms
#define CAR_SPEED_CONSTANT              1000.0 / SPEED_CONTROL_PERIOD / (float)ENCODE_CONSTANT
#define CAR_SPEED_SET                   g_fCarSpeedSet //g_Argument.fCarSpeedSet;

#define SPEED_CONTROL_OUT_MAX           0.1
#define SPEED_CONTROL_OUT_MIN           -1.0
/****************************************************/
#define ZHONGZHI 0


typedef struct PID
{
    float P;
    float I;
    float D;
}PID;


extern volatile float  g_f16AngleControlOut;
extern volatile float  g_fDirectionControlOut;   //×ª½Ç
extern volatile float  g_fCarDirectionSet;

extern volatile float  g_fCarSpeed;
extern volatile float  g_fCarSpeedSet;
extern volatile float  g_fSpeedControlOut;
extern volatile float  g_fSpeedControlOutNew, g_fSpeedControlOutOld, g_fSpeedControlIntegral;

extern volatile u8     g_nSpeedControlCount;
extern volatile u8     g_nSpeedControlPeriod;

extern volatile vs16   g_sMotorPuls;
extern volatile vs16   g_sMotorPulsSigma;

extern volatile float g_fLeft, g_fRight;


/******************************************************************************
* Copyright (c) 2016, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
void PID_INIT(void);

void GetAngle(void);

void PWM_Motor(int pwm_l,int pwm_r);

void GetMotorPuise(void);

void BANLANCE_PWM_OUT(float angel);

/*************************************************************************************/
void DirectionControlOut(void);
void AngleControlOut(void);
void SpeedControl(void);
void SpeedControlOutput(void);

void MotorOutput(void);

#endif

