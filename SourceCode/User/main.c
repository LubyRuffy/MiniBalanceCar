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
#include "SysTick.h"
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
    
    Encoder_Init_TIM2();            //=====编码器接口
    Encoder_Init_TIM4();
    
    /* 配置SysTick 为1ms中断一次 */
    SysTick_Init();
    Delay_ms(10);
    
    if(MPU6050_Init())
    {
        printf("MPU6050_Init Error!\r\n");
    }
    else
    {
        TIMER6_Config();
    }
    
    while(1)
    {
        //printf("{A%d:%d:%d:%d}$",a,b,c,d); 
        //a、b、c、d分别为需要显示的状态，
        //a对应左边的码盘，
        //b对应右边的码盘，通过百分比表示，可以用于速度等状态的显示，范围是0—100（%）；
        //c主要是用于显示电量，范围也是0—100（%）；
        //d是角度值，范围是-180°—180°
        printf("{A%d:%d:%d:%d}$",0,0,100,(int)(Pitch)); 
        Delay_ms(10);
        
    }
}


/*********************************************END OF FILE**********************/
