/**
******************************************************************************
* @file    LED.c
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar LED driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "LED.h"

/***************************************************************
*  Function name:   LED_GPIO_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void LED_GPIO_Config(void)
{       
        /*定义一个GPIO_InitTypeDef类型的结构体*/
        GPIO_InitTypeDef GPIO_InitStructure;

        /*开启GPIOB和GPIOF的外设时钟*/
        RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE); 

        /*选择要控制的GPIOB引脚*/                                                              
        GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN; 

        /*设置引脚模式为通用推挽输出*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

        /*设置引脚速率为50MHz */   
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

        /*调用库函数，初始化GPIOB0*/
        GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);  
        
        /* 关闭所有led灯    */
        //GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
        GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}
