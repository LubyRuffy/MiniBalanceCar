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
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启GPIO的外设时钟*/
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
    
    /* 设置TIM3CLK 为 72MHZ */
    //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOA clock enable */
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE); 

    /*GPIOA Configuration: TIM3 channel 1 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*GPIOB Configuration: TIM3 channel 2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 设置初始状态 */
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
    TIM_TimeBaseStructure.TIM_Period = 719;       //当定时器从0计数到719，即为720次，为一个定时周期
    TIM_TimeBaseStructure.TIM_Prescaler = 9;      //设置预分频：72MHz/10
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;  //设置时钟分频系数：不分频(这里用不到)
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;     //配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = 0;    //设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);    //使能通道1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;   //设置通道2的电平跳变值，输出另外一个占空比的PWM
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);     //使能通道2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);                   //使能定时器3 
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
