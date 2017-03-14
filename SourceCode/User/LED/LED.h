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


/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LED1_GPIO_PORT          GPIOB                       /* GPIO�˿� */
#define LED1_GPIO_CLK           RCC_APB2Periph_GPIOB        /* GPIO�˿�ʱ�� */
#define LED1_GPIO_PIN           GPIO_Pin_12                 /* ���ӵ�SCLʱ���ߵ�GPIO */


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)          {p->BSRR=i;}			//����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)          {p->BRR=i;}				//����͵�ƽ
#define digitalToggle(p,i)      {p->ODR ^=i;}			//�����ת״̬

/* �������IO�ĺ� */
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
