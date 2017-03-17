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


/***************************************************************
* Function name:  I2C_GPIO_Config()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
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


/***************************************************************
* Function name:  I2C_Mode_Config()
* Description:     
* Entry:          None
*
* Returned value: None
* Remark:
***************************************************************/
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



void IIC_Write_Buffer(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data)
{
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 

    /* Send START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT)); 

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2Cx, slave_addr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

    /* Send the EEPROM's internal address to write to */    
    I2C_SendData(I2Cx, reg_addr);  

    /* Test on EV8 and clear it */
    while(! I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* While there is data to be written */
    while(length--)  
    {
    /* Send the current byte */
    I2C_SendData(I2Cx, *data); 

    /* Point to the next byte to be written */
    data++; 

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    /* Send STOP condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
}

void IIC_Read_Buffer(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data)
{
    //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)); 
    
    /* Send STRAT condition */
    I2C_GenerateSTART(I2Cx, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;
    
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
    
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2Cx, slave_addr, I2C_Direction_Transmitter);
    
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2Cx, ENABLE);
    
    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2Cx, reg_addr);  

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2Cx, slave_addr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    while(length)  
    {
        if(length == 1)
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
            *data = I2C_ReceiveData(I2Cx);

            /* Point to the next location where the byte read will be saved */
            data++; 

            /* Decrement the read bytes counter */
            length--;        
        }   
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
}

/*********************************************END OF FILE**********************/
