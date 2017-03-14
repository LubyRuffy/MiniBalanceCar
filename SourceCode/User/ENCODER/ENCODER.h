/**
******************************************************************************
* @file    ENCODER.h
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar ENCODER driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __ENCODER_H__
#define	__ENCODER_H__
#include "stm32f10x.h"



#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�


/***************************************************************
*  Function name:   Encoder_Init_TIM2()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Encoder_Init_TIM2(void);

/***************************************************************
*  Function name:   Encoder_Init_TIM4()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Encoder_Init_TIM4(void);

/***************************************************************
*  Function name:   Read_Encoder()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
int Read_Encoder(u8 TIMX);

#endif
/*********************************************END OF FILE**********************/
