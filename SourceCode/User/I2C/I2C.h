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

/**************************I2C�������壬I2C1��I2C2********************************/
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

//addr������slave_address
//ReadAddr ��     ��������Ҫ�������ݵ��׵�ַ
//NumByteToRead ���������ݵĳ���
//pBuffer ��      ��Ҫ���������ݴ洢λ��
void IIC_Read_Buffer(u8 ReadAddr, u8* pBuffer, u8 NumByteToRead);

/**
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
void IIC_Write_Buffer(u8 WriteAddr, u8 NumByteToWrite, u8* pBuffer);
void IIC_Write(u8 WriteAddr, u8 Buffer);
#endif
/*********************************************END OF FILE**********************/
