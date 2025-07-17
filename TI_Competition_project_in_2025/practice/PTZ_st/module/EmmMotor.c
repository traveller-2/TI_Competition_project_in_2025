#include "EmmMotor.h"
#include <stdlib.h>
#include <string.h>

#define CHECKSUM_FIXED 0x6B

// 注册电机实例
EmmMotorInstance *EmmMotorRegister(USARTInstance *usart, uint8_t address) {
    EmmMotorInstance *motor = malloc(sizeof(EmmMotorInstance));
    if (!motor) return NULL;
    motor->usart = usart;
    motor->address = address;
    motor->is_enabled = false;
    motor->user_data = NULL;

    usart->onReceive = EmmMotor_USART_Callback;
    usart->parent = motor;
    return motor;
}

// 电机使能/失能
bool EmmMotor_Enable(EmmMotorInstance *motor, bool enable) {
    uint8_t frame[6] = {
        motor->address,
        0xF3,
        0xAB,
        enable ? 0x01 : 0x00,
        0x00,            // 多机同步标志
        CHECKSUM_FIXED
    };
    motor->is_enabled = enable;
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// 速度模式控制
bool EmmMotor_SetSpeed(EmmMotorInstance *motor, bool ccw, uint16_t rpm, uint8_t acc) {
    uint8_t frame[8];
    frame[0] = motor->address;
    frame[1] = 0xF6;
    frame[2] = ccw ? 0x01 : 0x00;
    frame[3] = (rpm >> 8) & 0xFF;
    frame[4] = rpm & 0xFF;
    frame[5] = acc;
    frame[6] = 0x00;  // 不启用多机同步
    frame[7] = CHECKSUM_FIXED;
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// 停止命令
bool EmmMotor_Stop(EmmMotorInstance *motor) {
    uint8_t frame[5] = {
        motor->address,
        0xFE,
        0x98,
        0x00,  // 不启用多机同步
        CHECKSUM_FIXED
    };
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// 触发多机同步开始
bool EmmMotor_SyncStart(void) {
    uint8_t frame[4] = {
        0x00,  // 广播地址
        0xFF,
        0x66,
        CHECKSUM_FIXED
    };
    // 这里应广播到所有注册过的 motor 实例
    // 这里只做一帧发送
    return USARTSendArray(NULL /*你可以传全局 usart 或 motor->usart*/, frame, sizeof(frame));
}

// 原点回零
bool EmmMotor_TriggerZeroing(EmmMotorInstance *motor) {
    uint8_t frame[5] = {
        motor->address,
        0x9A,
        0x00,  // 回零模式
        0x00,  // 不启用多机同步
        CHECKSUM_FIXED
    };
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// 串口接收回调
void EmmMotor_USART_Callback(USARTInstance *usart, uint8_t data) {
    EmmMotorInstance *motor = (EmmMotorInstance *)usart->parent;
    // 处理接收到的字节：可加入状态机解析返回帧，或者 ringbuffer 分析
    (void)motor;
    (void)data;
}
