#ifndef MODULE_EMM_MOTOR_H
#define MODULE_EMM_MOTOR_H

#include "bsp_usart.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct EmmMotorInstance EmmMotorInstance;

struct EmmMotorInstance {
    USARTInstance *usart;
    uint8_t address;      // 电机地址
    bool is_enabled;
    void *user_data;      // 可扩展字段，如附加回调、状态等
};

// 注册电机实例
EmmMotorInstance *EmmMotorRegister(USARTInstance *usart, uint8_t address);

// 基本控制命令
bool EmmMotor_Enable(EmmMotorInstance *motor, bool enable);
bool EmmMotor_SetSpeed(EmmMotorInstance *motor, bool ccw, uint16_t rpm, uint8_t acc);
bool EmmMotor_Stop(EmmMotorInstance *motor);
bool EmmMotor_SyncStart(void);
bool EmmMotor_TriggerZeroing(EmmMotorInstance *motor);

// 接收回调（注册时使用）
void EmmMotor_USART_Callback(USARTInstance *usart, uint8_t data);

#endif
