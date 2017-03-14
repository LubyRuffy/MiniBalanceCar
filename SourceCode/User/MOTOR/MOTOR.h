/**
******************************************************************************
* @file    MOTOR.h
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar MOTOR driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __MOTOR_H__
#define	__MOTOR_H__
#include "stm32f10x.h"


#define MOTOR_OUT_MAX           0.999

/* 定义Motor连接的GPIO端口 */
#define STBY_GPIO_PORT       GPIOF                       /* GPIO端口 */
#define STBY_GPIO_CLK        RCC_APB2Periph_GPIOF        /* GPIO端口时钟 */
#define STBY_GPIO_PIN        GPIO_Pin_12                 /* 连接到STBY的GPIO */

#define AIN1_GPIO_PORT       GPIOB                       /* GPIO端口 */
#define AIN1_GPIO_CLK        RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define AIN1_GPIO_PIN        GPIO_Pin_0                  /* 连接到AIN1的GPIO */

#define AIN2_GPIO_PORT       GPIOB                       /* GPIO端口 */
#define AIN2_GPIO_CLK        RCC_APB2Periph_GPIOB        /* GPIO端口时钟 */
#define AIN2_GPIO_PIN        GPIO_Pin_1                  /* 连接到AIN2的GPIO */

#define BIN1_GPIO_PORT       GPIOF                       /* GPIO端口 */
#define BIN1_GPIO_CLK        RCC_APB2Periph_GPIOF        /* GPIO端口时钟 */
#define BIN1_GPIO_PIN        GPIO_Pin_11                 /* 连接到BIN1的GPIO */

#define BIN2_GPIO_PORT       GPIOF                       /* GPIO端口 */
#define BIN2_GPIO_CLK        RCC_APB2Periph_GPIOF        /* GPIO端口时钟 */
#define BIN2_GPIO_PIN        GPIO_Pin_13                 /* 连接到BIN2的GPIO */


/* 带参宏，可以像内联函数一样使用 */
#define STBY(a)     if (a)  \
                    GPIO_SetBits(STBY_GPIO_PORT,STBY_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(STBY_GPIO_PORT,STBY_GPIO_PIN)

#define AIN1(a)     if (a)  \
                    GPIO_SetBits(AIN1_GPIO_PORT,AIN1_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(AIN1_GPIO_PORT,AIN1_GPIO_PIN)
#define AIN2(a)     if (a)  \
                    GPIO_SetBits(AIN2_GPIO_PORT,AIN2_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(AIN2_GPIO_PORT,AIN2_GPIO_PIN)

#define BIN1(a)     if (a)  \
                    GPIO_SetBits(BIN1_GPIO_PORT,BIN1_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(BIN1_GPIO_PORT,BIN1_GPIO_PIN)

#define BIN2(a)     if (a)  \
                    GPIO_SetBits(BIN2_GPIO_PORT,BIN2_GPIO_PIN);\
                    else        \
                    GPIO_ResetBits(BIN2_GPIO_PORT,BIN2_GPIO_PIN)


/***************************************************************
*  Function name:   Motor_GPIO_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_GPIO_Config(void);
                    
/***************************************************************
*  Function name:   Motor_PWM_Config()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_PWM_Config(void);

/***************************************************************
*  Function name:   Motor_driver_out()
*  Description:     
*  Entry:           None
*
*  Returned value:  None
*  Remark:
***************************************************************/
void Motor_driver_out(float LeftVoltage, float RightVoltage);
                    
#endif
/*********************************************END OF FILE**********************/
