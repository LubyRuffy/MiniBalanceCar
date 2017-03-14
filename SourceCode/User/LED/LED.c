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
        /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
        GPIO_InitTypeDef GPIO_InitStructure;

        /*����GPIOB��GPIOF������ʱ��*/
        RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE); 

        /*ѡ��Ҫ���Ƶ�GPIOB����*/                                                              
        GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN; 

        /*��������ģʽΪͨ���������*/
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

        /*������������Ϊ50MHz */   
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

        /*���ÿ⺯������ʼ��GPIOB0*/
        GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);  
        
        /* �ر�����led��    */
        //GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
        GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}
