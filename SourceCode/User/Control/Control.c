/**
******************************************************************************
* @file    Control.c
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar minibalance control source file
* @history    
*
* Copyright (c) 2016, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "Control.h"
#include "LED.h"
#include "MOTOR.h"
#include "ENCODER.h"
#include "MPU6050.h"

PID PID_KONGZHI;
PID PID_Speed;


volatile float  g_f16AngleControlOut;
volatile float  g_fDirectionControlOut;   //转角

volatile float  g_fCarSpeed = 0;
volatile float  g_fCarSpeedSet = 0;
volatile float  g_fSpeedControlOut = 0;
volatile float  g_fSpeedControlOutNew, g_fSpeedControlOutOld, g_fSpeedControlIntegral;

volatile u8     g_nSpeedControlCount = 0;
volatile u8     g_nSpeedControlPeriod = 0;

volatile vs16   g_sMotorPuls = 0;
volatile vs16   g_sMotorPulsSigma = 0;

volatile float g_fLeft, g_fRight;

void PID_INIT(void)
{ 
    #if 1
    PID_KONGZHI.P = 0.075000;       // 角度控制系数
    PID_KONGZHI.D = 0.000080;     //  角速度控制系数 0.000080
    #else
    PID_KONGZHI.P = 0;       // 角度控制系数
    PID_KONGZHI.D = 0;     //  角速度控制系数 
    #endif

    #if 1
    PID_Speed.P = 0.115;        //速度控制系数  0.125;
    PID_Speed.I = 0.015;        //位移控制系数  0.0125;
    #else
    PID_Speed.P = 0;        //速度控制系数  
    PID_Speed.I = 0;        //位移控制系数  0.025
    #endif
}

void GPIO_BANLANCE_INIT(void)
{
    RCC->APB2ENR|=1<<3;             //先使能外设IO PORTB时钟                             
    RCC->APB2ENR|=1<<7;             //先使能外设IO PORTF时钟
    
    GPIOB->CRL&=0XFFFFFF00;     
    GPIOB->CRL|=0X00000033;         //PB0/1 通用推挽输出 50Hz   
    
    GPIOF->CRH&=0XFF000FFF;     
    GPIOF->CRH|=0X00333000;         //PF11/12/13 通用推挽输出 50Hz  
    
    GPIOB->ODR|=0<<0;     //PB0 输出低
    GPIOB->ODR|=0<<1;     //PB1 输出低
    GPIOF->ODR|=0<<11;     //PB2 输出低
    GPIOF->ODR|=0<<13;     //PB2 输出低
    GPIOF->ODR|=1<<12;     //PF12 输出高
}

void GetAngle(void)
{
    MPU6050_Refresh_Pose();
}

void AngleControlOut(void)
{
    float fValue,fValue1 ;

    fValue = PID_KONGZHI.P * Pitch;
    fValue1 = PID_KONGZHI.D * (gyro[1]);
    
    g_f16AngleControlOut = fValue1 + fValue;
}

void GetMotorPuise(void)
{
    g_sMotorPuls =(signed int)(Read_Encoder(2) + Read_Encoder(4));
    
    g_sMotorPulsSigma += g_sMotorPuls;
}

void SpeedControl(void) 
{
    float fP, fDelta;
    float fI;   
    //--------------------------------------------------------------------------
    g_fCarSpeed = -(g_sMotorPulsSigma>>1);
    g_sMotorPulsSigma = 0;
    g_fCarSpeed *= CAR_SPEED_CONSTANT;
    
    
    //--------------------------------------------------------------------------
    fDelta = CAR_SPEED_SET;
    fDelta -= g_fCarSpeed;
    
    fP = fDelta * PID_Speed.P;
    fI = fDelta * PID_Speed.I;
    //------------------------------------------------------------------
    if(fI < (-SPEED_CONTROL_MAX * PID_Speed.I) || fI > (SPEED_CONTROL_MAX * PID_Speed.I))
    {
        fI = 0; 
    }
    //--------------------------------------------------------------------------
    g_fSpeedControlIntegral += fI;
    //------------------------------------------------------------------
    
    if(g_fSpeedControlIntegral > SPEED_CONTROL_OUT_MAX) 
    g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MAX;
    if(g_fSpeedControlIntegral < SPEED_CONTROL_OUT_MIN)     
    g_fSpeedControlIntegral = SPEED_CONTROL_OUT_MIN;
    
    //--------------------------------------------------------------------------
    g_fSpeedControlOutOld = g_fSpeedControlOutNew;
    
    g_fSpeedControlOutNew = fP + g_fSpeedControlIntegral;

}

void SpeedControlOutput(void)
{
    float fValue;
    
    fValue = g_fSpeedControlOutNew - g_fSpeedControlOutOld;
    
    g_nSpeedControlPeriod++;
    
    fValue = fValue * g_nSpeedControlPeriod;
    fValue = fValue / SPEED_CONTROL_PERIOD;
    
    g_fSpeedControlOut = fValue + g_fSpeedControlOutOld;
}

void MotorOutput(void)
{
    g_fLeft  = g_f16AngleControlOut - g_fSpeedControlOut - g_fDirectionControlOut;
    g_fRight = g_f16AngleControlOut - g_fSpeedControlOut + g_fDirectionControlOut;

    Motor_driver_out(g_fLeft, g_fRight);            
}


