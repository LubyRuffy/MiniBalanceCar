/**
******************************************************************************
* @file    TIMER6.c
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar TIMER6 driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "TIMER6.h"


/***************************************************************
*  Function name:   TIMx_NVIC_Configuration()
*  Description:     TIMER6中断优先级配置
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             // 设置中断组为0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // 设置中断来源	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 设置抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIMER6的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIMER6的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler        都有
 *	TIM_CounterMode       TIMER6没有，其他都有
 *  TIM_Period            都有
 *  TIM_ClockDivision     TIMER6没有，其他都有
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
/***************************************************************
*  Function name:   TIMx_Configuration()
*  Description:     TIMER6配置
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void TIMER6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);// 开启TIMx_CLK,x[6,7],即内部时钟CK_INT=72M
    TIM_TimeBaseStructure.TIM_Period=1000;      //自动重装载寄存器周的值(计数值)
    TIM_TimeBaseStructure.TIM_Prescaler= 71;    // 时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     // 时钟分频因子
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; // 计数器计数模式
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;            // 重复计数器的值
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // 初始化定时器TIMx, x[6,7]
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // 清除计数器中断标志位
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // 开启计数器中断
	
    TIMx_NVIC_Configuration();
	
    TIM_Cmd(TIM6, ENABLE);	                                    // 使能计数器																	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);      // 暂时关闭TIM6的时钟，等待使用
}
/*********************************************END OF FILE**********************/
