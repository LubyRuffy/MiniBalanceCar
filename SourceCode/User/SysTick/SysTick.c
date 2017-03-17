/**
******************************************************************************
* @file    SysTick.c
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar SysTick driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "SysTick.h"


static __IO u32 TimingDelay;

/***************************************************************
* Function name:  SysTick_Init()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/***************************************************************
* Function name:  Delay_ms()
* Description:     
* Entry:          nTime: 则实现的延时为 nTime * 1ms
*
* Returned value: None
* Remark:
***************************************************************/
void Delay_ms(unsigned long nTime)
{
	TimingDelay = nTime;
    
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
    
	while(TimingDelay != 0);
}

/***************************************************************
* Function name:  get_clock_ms()
* Description:    
* Entry:          Timestamp
*
* Returned value: None
* Remark:
***************************************************************/
int get_clock_ms(unsigned long *count)
{
    if (!count)
        return 1;
    count[0] = TimingDelay;
    return 0;
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}


/*********************************************END OF FILE**********************/
