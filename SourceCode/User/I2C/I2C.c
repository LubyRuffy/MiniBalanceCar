/**
******************************************************************************
* @file    I2C.c
* @author  dwu
* @version V1.0.0
* @date    6-Mar-2017
* @brief   MiniBalanceCar I2C driver source file
* @history    
*
* Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "I2C.h"


#define I2C_Speed               400000  
#define I2Cx_OWN_ADDRESS7       0x0A   
#define MPU6050_ADDRESS         0xD0

void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 

    /* 使能与 I2C1 有关的时钟 */
    I2C_APBxClock_FUN ( I2C_CLK, ENABLE );
    I2C_GPIO_APBxClock_FUN ( I2C_GPIO_CLK, ENABLE );


    /* PB6-I2C1_SCL、PB7-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;     // 开漏输出
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;     // 开漏输出
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
}


/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C1 初始化 */
  I2C_Init(I2Cx, &I2C_InitStructure);
  
	/* 使能 I2C1 */
  I2C_Cmd(I2Cx, ENABLE);   
}


//ReadAddr ：     从器件将要读的数据的首地址
//NumByteToRead ：读出数据的长度
//pBuffer ：      将要读出的数据存储位置
void IIC_Read_Buffer(u8 ReadAddr, u8* pBuffer, u8 NumByteToRead)
{
    //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 
    
    /* Send STRAT condition */
    I2C_GenerateSTART(I2Cx, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2Cx, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2Cx, ENABLE);
    
    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2Cx, ReadAddr);  

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2Cx, MPU6050_ADDRESS, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2Cx, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2Cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2Cx);

            /* Point to the next location where the byte read will be saved */
            pBuffer++; 

            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
}

void IIC_Write(u8 WriteAddr, u8 Buffer)
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 

    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); 

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2Cx, MPU6050_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

    /* Send the EEPROM's internal address to write to */    
    I2C_SendData(I2Cx, WriteAddr);  

    /* Test on EV8 and clear it */
    while(! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* While there is data to be written */
    /* Send the current byte */
    I2C_SendData(I2Cx, Buffer); 

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void IIC_Write_Buffer(u8 WriteAddr, u8 NumByteToWrite, u8* pBuffer)
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 

    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); 

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2Cx, MPU6050_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

    /* Send the EEPROM's internal address to write to */    
    I2C_SendData(I2Cx, WriteAddr);  

    /* Test on EV8 and clear it */
    while(! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* While there is data to be written */
    while(NumByteToWrite--)  
    {
    /* Send the current byte */
    I2C_SendData(I2Cx, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

/*********************************************END OF FILE**********************/
