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
    
    Encoder_Init_TIM2();            //=====�������ӿ�
    Encoder_Init_TIM4();
    
    /* ����SysTick Ϊ1ms�ж�һ�� */
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
        //a��b��c��d�ֱ�Ϊ��Ҫ��ʾ��״̬��
        //a��Ӧ��ߵ����̣�
        //b��Ӧ�ұߵ����̣�ͨ���ٷֱȱ�ʾ�����������ٶȵ�״̬����ʾ����Χ��0��100��%����
        //c��Ҫ��������ʾ��������ΧҲ��0��100��%����
        //d�ǽǶ�ֵ����Χ��-180�㡪180��
        printf("{A%d:%d:%d:%d}$",0,0,100,(int)(Pitch)); 
        Delay_ms(10);
        
    }
}


/*********************************************END OF FILE**********************/
