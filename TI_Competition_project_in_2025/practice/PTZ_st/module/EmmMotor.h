#ifndef MODULE_EMM_MOTOR_H
#define MODULE_EMM_MOTOR_H

#include "bsp_usart.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct EmmMotorInstance EmmMotorInstance;

struct EmmMotorInstance {
    USARTInstance *usart;
    uint8_t address;      // �����ַ
    bool is_enabled;
    void *user_data;      // ����չ�ֶΣ��總�ӻص���״̬��
};

// ע����ʵ��
EmmMotorInstance *EmmMotorRegister(USARTInstance *usart, uint8_t address);

// ������������
bool EmmMotor_Enable(EmmMotorInstance *motor, bool enable);
bool EmmMotor_SetSpeed(EmmMotorInstance *motor, bool ccw, uint16_t rpm, uint8_t acc);
bool EmmMotor_Stop(EmmMotorInstance *motor);
bool EmmMotor_SyncStart(void);
bool EmmMotor_TriggerZeroing(EmmMotorInstance *motor);

// ���ջص���ע��ʱʹ�ã�
void EmmMotor_USART_Callback(USARTInstance *usart, uint8_t data);

#endif
