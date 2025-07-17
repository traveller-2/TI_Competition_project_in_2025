#ifndef __SOFT_IIC_H
#define __SOFT_IIC_H

#include "stm32f4xx_hal.h"

//========= �������� =========
/*********************************************************************************************
	*������ʹ�����iic������
	*PC13 ~ PC15									���� GPIO
	*PE7 / PE8 / PE9 ~ PE15				���ù��ܸ���
	*PB2													����Ϊ BOOT1
	*PD13 ~ PD15									Ĭ������ LED �� FSMC
	*PA13 / PA14									SWD��������
	*PF0 / PF1										�ڲ�����ʱ������
	*PH0 / PH1										�ⲿ���پ���HSE������
	*�������ڲ�����								ĳЩ���ţ���һ���� PE/PG����ʹ���� GPIO_PULLUP��Ҳ�޷�����������ƽ
	********************************************************************************************/
#define IIC_SDA_GPIO_PORT   GPIOC
#define IIC_SDA_GPIO_PIN    GPIO_PIN_0

#define IIC_SCL_GPIO_PORT   GPIOC
#define IIC_SCL_GPIO_PIN    GPIO_PIN_1

#define IIC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()

//========= ���ſ��� =========
#define SDA_HIGH()    HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET)
#define SDA_LOW()     HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET)
#define SCL_HIGH()    HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET)
#define SCL_LOW()     HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET)
#define READ_SDA()    HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)

//========= �ӿں��� =========
void SoftIIC_Init(void);
void SoftIIC_Start(void);
void SoftIIC_Stop(void);
void SoftIIC_SendByte(uint8_t data);
uint8_t SoftIIC_ReadByte(uint8_t ack);
uint8_t SoftIIC_WaitAck(void);
void SoftIIC_Ack(void);
void SoftIIC_NAck(void);

#endif

