#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"
#include "tim.h"
#include "stdint.h"

#define MAX_SERVO_NUM  2  // ���֧��2�����
extern volatile uint16_t servo_angle[MAX_SERVO_NUM] ;

void Servo_ReadAngleOnce(uint8_t id);//����Ƕȶ�ȡ


/**
 * �����ʼ������
 * ���ڳ�ʼ�����кʹ����������񣬱�����ϵͳ���������һ��
 */
void Servo_Init(void);



/**
 * ������������ڲ�ʹ�ã�
 * ����Ҫ�û����ã��� Servo_Init �Զ�����
 */
void ServoControlTask(void *argument);



/**
 * ����ʽ������ƺ������Ƽ�ʹ�ã�
 * ���������������Ϣ���У��ɺ�̨����˳��ִ�У����⴮�ڳ�ͻ
 *
 * @param id       ��� ID����Χ 0~254��
 * @param pwm      α PWM ֵ����Χ 500~2500��ʵ��ΪĿ��λ�ã�
 * @param time_ms  �������ʱ�䣨��λ ms����Χ 0~9999��
 */
void Servo_QueueControl(uint8_t id, uint16_t pwm, uint16_t time_ms);



/**
 * ���ö�� ID
 * @param old_id   ԭ��� ID
 * @param new_id   �� ID������Ψһ
 * ?? �޸� ID ʱ��ֻ����һ����������ܴ���
 */
void Servo_SetID(uint8_t old_id, uint8_t new_id);



/**
 * ���ö������ģʽ��1~8��
 * ����ģʽ�����˵���飬�� 1=270��˳ʱ�룬5=��Ȧ˳ʱ���
 */
void Servo_SetMode(uint8_t id, uint8_t mode);



/**
 * ��ȡ�����ǰ�Ƕ�ֵ��αPWMֵ�������ظ�ʽ�磺#001P1500!
 * ?? ��Ҫ TXD �� RXD �̽Ӳ����յ�����ֵ
 */
void Servo_ReadAngle(uint8_t id, char *ret_buf, uint16_t timeout);



/**
 * ������Ͽ���
 * ������ʽΪƴ�ӵĶ���������� {}�������������Զ����� {G0000 ... }
 * ʾ����
 *   Servo_MultiControl("#000P1500T1000!#001P2500T0500!");
 */
void Servo_MultiControl(const char *commands);

////////////////////////////////////////////////////////�±���PWM���

// ��ʼ�� PWM
void servo_init(void);

// ���ýǶȣ�0~180�㣩
void servo_set_angle(uint8_t angle);

// �����������
void ServoTask(void *argument);

#endif
