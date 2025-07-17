#include "EmmMotor.h"
#include <stdlib.h>
#include <string.h>

#define CHECKSUM_FIXED 0x6B

// ע����ʵ��
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

// ���ʹ��/ʧ��
bool EmmMotor_Enable(EmmMotorInstance *motor, bool enable) {
    uint8_t frame[6] = {
        motor->address,
        0xF3,
        0xAB,
        enable ? 0x01 : 0x00,
        0x00,            // ���ͬ����־
        CHECKSUM_FIXED
    };
    motor->is_enabled = enable;
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// �ٶ�ģʽ����
bool EmmMotor_SetSpeed(EmmMotorInstance *motor, bool ccw, uint16_t rpm, uint8_t acc) {
    uint8_t frame[8];
    frame[0] = motor->address;
    frame[1] = 0xF6;
    frame[2] = ccw ? 0x01 : 0x00;
    frame[3] = (rpm >> 8) & 0xFF;
    frame[4] = rpm & 0xFF;
    frame[5] = acc;
    frame[6] = 0x00;  // �����ö��ͬ��
    frame[7] = CHECKSUM_FIXED;
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// ֹͣ����
bool EmmMotor_Stop(EmmMotorInstance *motor) {
    uint8_t frame[5] = {
        motor->address,
        0xFE,
        0x98,
        0x00,  // �����ö��ͬ��
        CHECKSUM_FIXED
    };
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// �������ͬ����ʼ
bool EmmMotor_SyncStart(void) {
    uint8_t frame[4] = {
        0x00,  // �㲥��ַ
        0xFF,
        0x66,
        CHECKSUM_FIXED
    };
    // ����Ӧ�㲥������ע����� motor ʵ��
    // ����ֻ��һ֡����
    return USARTSendArray(NULL /*����Դ�ȫ�� usart �� motor->usart*/, frame, sizeof(frame));
}

// ԭ�����
bool EmmMotor_TriggerZeroing(EmmMotorInstance *motor) {
    uint8_t frame[5] = {
        motor->address,
        0x9A,
        0x00,  // ����ģʽ
        0x00,  // �����ö��ͬ��
        CHECKSUM_FIXED
    };
    return USARTSendArray(motor->usart, frame, sizeof(frame));
}

// ���ڽ��ջص�
void EmmMotor_USART_Callback(USARTInstance *usart, uint8_t data) {
    EmmMotorInstance *motor = (EmmMotorInstance *)usart->parent;
    // ������յ����ֽڣ��ɼ���״̬����������֡������ ringbuffer ����
    (void)motor;
    (void)data;
}
