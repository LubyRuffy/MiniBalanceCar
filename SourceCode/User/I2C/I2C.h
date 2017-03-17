/**
******************************************************************************
* @file    I2C.h
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar I2C driver define
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __I2C_H__
#define	__I2C_H__
#include "stm32f10x.h"

/**************************I2C参数定义，I2C1或I2C2********************************/
#define             I2Cx                                I2C2
#define             I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             I2C_CLK                             RCC_APB1Periph_I2C2
#define             I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             I2C_SCL_PORT                        GPIOB   
#define             I2C_SCL_PIN                         GPIO_Pin_10
#define             I2C_SDA_PORT                        GPIOB 
#define             I2C_SDA_PIN                         GPIO_Pin_11

/***************************************************************
* Function name:  I2C_GPIO_Config()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void I2C_GPIO_Config(void);

/***************************************************************
* Function name:  I2C_Mode_Config()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
void I2C_Mode_Config(void);

void IIC_Write_Buffer(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
void IIC_Read_Buffer(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);

#endif
/*********************************************END OF FILE**********************/
