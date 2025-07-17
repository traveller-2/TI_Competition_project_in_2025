#ifndef __MOTOR_IIC_H
#define __MOTOR_IIC_H

#include <stdint.h>
#include "soft_iic.h"
#include "motor_iic.h"  // �Լ����� motor_iic.h����������
#include "cmsis_os.h"
#include <stdio.h>
#include "semphr.h"

#define MOTOR_I2C_ADDR 0x34
#define CAM_DEFAULT_I2C_ADDRESS       (0x34)			//I2C��ַ
#define MOTOR_TYPE_ADDR               0x14 				//�������������üĴ�����ַ
#define MOTOR_FIXED_SPEED_ADDR       	0x33				//�ٶȼĴ�����ַ�����ڱջ�����
#define MOTOR_ENCODER_POLARITY_ADDR   0x15				//������뷽���Ե�ַ
#define MOTOR_FIXED_PWM_ADDR      		0x1F				//�̶�PWM���Ƶ�ַ�����ڿ�������
#define MOTOR_ENCODER_TOTAL_ADDR  		0x3C				//4�����������Ե�������ֵ
#define ADC_BAT_ADDR                  0x00				//��ѹ��ַ

#define MOTOR_TYPE_WITHOUT_ENCODER        0
#define MOTOR_TYPE_TT                     1
#define MOTOR_TYPE_N20                    2
#define MOTOR_TYPE_JGB37_520_12V_110RPM   3

extern SemaphoreHandle_t xIIC_Mutex;
extern osMessageQueueId_t motorSpeedQueue;  // ��Ϣ���о�����������г�ʼ����
void IIC_Mutex_Init(void);

uint8_t Motor_I2C_Write(uint8_t reg, int8_t *buf, uint8_t len);
uint8_t Motor_I2C_Read(uint8_t reg, uint8_t *buf, uint8_t len);
void Motor_SetSpeed_Async(int8_t m1, int8_t m2, int8_t m3, int8_t m4);
void Motor_UpdateRPM(void);
void MotorControl_Task(void *argument);
void Motor_SetSpeed(int8_t speed[4]);
void Motor_Stop(void);
void Motor_Init(uint8_t motor_type, uint8_t polarity);
void Motor_ReadEncoder(int32_t encoder[4]);


#endif
