/**
******************************************************************************
* @file    MOTOR.c
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar MOTOR driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "MOTOR.h"



/***************************************************************
*  Function name:   Motor_GPIO_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_GPIO_Config(void)

{       
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����GPIO������ʱ��*/
    RCC_APB2PeriphClockCmd( STBY_GPIO_CLK | \
                            AIN1_GPIO_CLK | AIN2_GPIO_CLK | \
                            BIN1_GPIO_CLK | BIN2_GPIO_CLK, ENABLE); 

    /* STBY */                                                              
    GPIO_InitStructure.GPIO_Pin = STBY_GPIO_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(STBY_GPIO_PORT, &GPIO_InitStructure);

    /* AIN1 */                                                              
    GPIO_InitStructure.GPIO_Pin = AIN1_GPIO_PIN; 
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(AIN1_GPIO_PORT, &GPIO_InitStructure);

    /* AIN2 */                                                              
    GPIO_InitStructure.GPIO_Pin = AIN2_GPIO_PIN; 
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(AIN2_GPIO_PORT, &GPIO_InitStructure);

    /* BIN1 */                                                              
    GPIO_InitStructure.GPIO_Pin = BIN1_GPIO_PIN; 
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(BIN1_GPIO_PORT, &GPIO_InitStructure);

    /* BIN2 */                                                              
    GPIO_InitStructure.GPIO_Pin = BIN2_GPIO_PIN; 
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(BIN2_GPIO_PORT, &GPIO_InitStructure);
    
    /* ����TIM3CLK Ϊ 72MHZ */
    //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOA clock enable */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE); 

    /*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* ���ó�ʼ״̬ */
    GPIO_ResetBits(STBY_GPIO_PORT, STBY_GPIO_PIN);
    GPIO_ResetBits(AIN1_GPIO_PORT, AIN1_GPIO_PIN);
    GPIO_ResetBits(AIN2_GPIO_PORT, AIN2_GPIO_PIN);
    GPIO_ResetBits(BIN1_GPIO_PORT, BIN1_GPIO_PIN);
    GPIO_ResetBits(BIN2_GPIO_PORT, BIN2_GPIO_PIN);
}

/***************************************************************
*  Function name:   Motor_PWM_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_PWM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Time base configuration */      
    TIM_TimeBaseStructure.TIM_Period = 719;       //����ʱ����0������719����Ϊ720�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Prescaler = 9;      //����Ԥ��Ƶ��72MHz/10
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;  //����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     //����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = 0;    //��������ֵ�������������������ֵʱ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);    //ʹ��ͨ��1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;   //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);     //ʹ��ͨ��2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3 
}

/***************************************************************
*  Function name:   Motor_driver_out()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_driver_out(float LeftVoltage, float RightVoltage)
{   
    #define DEAD_VALUE_CLOCKWISE        0.0175
    #define DEAD_VALUE_ANTI_CLOCKWISE   0.025
    
    //LeftVoltage = RightVoltage = 0.001;
    
    //
    if(LeftVoltage > 0)
    {
        AIN1(0);
        AIN2(1);
        LeftVoltage += DEAD_VALUE_CLOCKWISE;
    }
    else
    {
        AIN1(1);
        AIN2(0);
        LeftVoltage = -LeftVoltage + DEAD_VALUE_ANTI_CLOCKWISE;
    }
    
    //right
    if(RightVoltage>0)
    {
        BIN1(1);
        BIN2(0);
        RightVoltage += DEAD_VALUE_ANTI_CLOCKWISE;
    }
    else
    {
        BIN1(0);
        BIN2(1);
        RightVoltage = -RightVoltage + DEAD_VALUE_CLOCKWISE;
    } 
    
    if(LeftVoltage  > MOTOR_OUT_MAX){LeftVoltage  = MOTOR_OUT_MAX;}    
    if(RightVoltage > MOTOR_OUT_MAX){RightVoltage = MOTOR_OUT_MAX;}
    
    TIM3->CCR1=(LeftVoltage  * 720);   
    TIM3->CCR2=(RightVoltage * 720); 
}

/*********************************************END OF FILE**********************/
