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

    /* ʹ���� I2C1 �йص�ʱ�� */
    I2C_APBxClock_FUN ( I2C_CLK, ENABLE );
    I2C_GPIO_APBxClock_FUN ( I2C_GPIO_CLK, ENABLE );


    /* PB6-I2C1_SCL��PB7-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;     // ��©���
    GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;     // ��©���
    GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);
}


/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
void I2C_Mode_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* ͨ������ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C1 ��ʼ�� */
  I2C_Init(I2Cx, &I2C_InitStructure);
  
	/* ʹ�� I2C1 */
  I2C_Cmd(I2Cx, ENABLE);   
}


//ReadAddr ��     ��������Ҫ�������ݵ��׵�ַ
//NumByteToRead ���������ݵĳ���
//pBuffer ��      ��Ҫ���������ݴ洢λ��
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
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
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
