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


void I2C_GPIO_Config(void);
void I2C_Mode_Config(void);

//addr：器件slave_address
//ReadAddr ：     从器件将要读的数据的首地址
//NumByteToRead ：读出数据的长度
//pBuffer ：      将要读出的数据存储位置
void IIC_Read_Buffer(u8 ReadAddr, u8* pBuffer, u8 NumByteToRead);

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void IIC_Write_Buffer(u8 WriteAddr, u8 NumByteToWrite, u8* pBuffer);
void IIC_Write(u8 WriteAddr, u8 Buffer);
#endif
/*********************************************END OF FILE**********************/
