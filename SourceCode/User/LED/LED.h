/**
******************************************************************************
* @file    LED.h
* @author  dwu
* @version V1.0.0
* @date    09-Nov-2016
* @brief   MiniBalanceCar LED driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __LED_H__
#define	__LED_H__
#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_PORT          GPIOB                       /* GPIO端口 */
#define LED1_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define LED1_GPIO_PIN           GPIO_Pin_12                 /* 连接到SCL时钟线的GPIO */


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)          {p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)          {p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)      {p->ODR ^=i;}			//输出反转状态

/* 定义控制IO的宏 */
#define LED1_TOGGLE             digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF                digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON                 digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)

/***************************************************************
*  Function name:   LED_GPIO_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void LED_GPIO_Config(void);

#endif
