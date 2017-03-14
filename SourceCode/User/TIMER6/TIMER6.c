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
*  Description:     TIMER6�ж����ȼ�����
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             // �����ж���Ϊ0	
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             // �����ж���Դ	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // ������ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIMER6�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIMER6��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler        ����
 *	TIM_CounterMode       TIMER6û�У���������
 *  TIM_Period            ����
 *  TIM_ClockDivision     TIMER6û�У���������
 *  TIM_RepetitionCounter TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
/***************************************************************
*  Function name:   TIMx_Configuration()
*  Description:     TIMER6����
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void TIMER6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);// ����TIMx_CLK,x[6,7],���ڲ�ʱ��CK_INT=72M
    TIM_TimeBaseStructure.TIM_Period=1000;      //�Զ���װ�ؼĴ����ܵ�ֵ(����ֵ)
    TIM_TimeBaseStructure.TIM_Prescaler= 71;    // ʱ��Ԥ��Ƶ��Ϊ71����������������ʱ��CK_CNT = CK_INT / (71+1)=1M
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;     // ʱ�ӷ�Ƶ����
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; // ����������ģʽ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;            // �ظ���������ֵ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);             // ��ʼ����ʱ��TIMx, x[6,7]
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);                       // ����������жϱ�־λ
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                    // �����������ж�
	
    TIMx_NVIC_Configuration();
	
    TIM_Cmd(TIM6, ENABLE);	                                    // ʹ�ܼ�����																	
    RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6, ENABLE);      // ��ʱ�ر�TIM6��ʱ�ӣ��ȴ�ʹ��
}
/*********************************************END OF FILE**********************/
