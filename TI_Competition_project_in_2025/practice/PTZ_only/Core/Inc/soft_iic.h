#ifndef __SOFT_IIC_H
#define __SOFT_IIC_H

#include "stm32f4xx_hal.h"

//========= 引脚配置 =========
/*********************************************************************************************
	*不建议使用软件iic的引脚
	*PC13 ~ PC15									低速 GPIO
	*PE7 / PE8 / PE9 ~ PE15				复用功能复杂
	*PB2													复用为 BOOT1
	*PD13 ~ PD15									默认用于 LED 或 FSMC
	*PA13 / PA14									SWD调试引脚
	*PF0 / PF1										内部用于时钟引脚
	*PH0 / PH1										外部高速晶振（HSE）引脚
	*引脚无内部上拉								某些引脚（如一部分 PE/PG）即使配置 GPIO_PULLUP，也无法真正上拉电平
	********************************************************************************************/
#define IIC_SDA_GPIO_PORT   GPIOC
#define IIC_SDA_GPIO_PIN    GPIO_PIN_0

#define IIC_SCL_GPIO_PORT   GPIOC
#define IIC_SCL_GPIO_PIN    GPIO_PIN_1

#define IIC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()

//========= 引脚控制 =========
#define SDA_HIGH()    HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)
#define SDA_LOW()     HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET)
#define SCL_HIGH()    HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET)
#define SCL_LOW()     HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET)
#define READ_SDA()    HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)

//========= 接口函数 =========
void SoftIIC_Init(void);
void SoftIIC_Start(void);
void SoftIIC_Stop(void);
void SoftIIC_SendByte(uint8_t data);
uint8_t SoftIIC_ReadByte(uint8_t ack);
uint8_t SoftIIC_WaitAck(void);
void SoftIIC_Ack(void);
void SoftIIC_NAck(void);

#endif

