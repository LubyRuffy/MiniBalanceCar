/**
******************************************************************************
* @file    main.c
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   
* @history 
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "LED.h"
#include "USART1.h"
#include "TIMER6.h"
#include "MOTOR.h"
#include "ENCODER.h"
#include "I2C.h"
#include "MPU6050.h"


/***************************************************************
* Function name:  main()
* Description:    main function    
* Entry:          None         
*  
* Returned value: None
* Remark:    
***************************************************************/
int main(void)
{   
    LED_GPIO_Config();
    USART1_Config();
    Motor_GPIO_Config();
    Motor_PWM_Config();
    
    Encoder_Init_TIM2();            //=====±àÂëÆ÷½Ó¿Ú
    Encoder_Init_TIM4();
    
    I2C_GPIO_Config();
    I2C_Mode_Config();
    MPU6050_Init();
    
    
    TIMER6_Config();
    
    while(1);
}


/*********************************************END OF FILE**********************/
