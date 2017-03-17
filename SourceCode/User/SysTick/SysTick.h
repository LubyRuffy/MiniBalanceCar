/**
******************************************************************************
* @file    SysTick.h
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar SysTick driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __SYSTICK_H__
#define	__SYSTICK_H__
#include "stm32f10x.h"


/***************************************************************
* Function name:  SysTick_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void SysTick_Init(void);

/***************************************************************
* Function name:  Delay_ms()
* Description:     
* Entry:          nTime: 则实现的延时为 nTime * 1ms
*
* Returned value: None
* Remark:
***************************************************************/
void Delay_ms(unsigned long nTime);

#endif
/*********************************************END OF FILE**********************/
