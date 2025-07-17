#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

// ǰ�������ṹ��
struct __USARTInstance;
typedef struct __USARTInstance USARTInstance;

// ���ջص�����ָ������
typedef void (*USARTRecvCallback)(USARTInstance *inst, uint8_t data);

// USART ʵ���ṹ��
struct __USARTInstance {
    UART_HandleTypeDef *huart;       // HAL���ھ��
    USARTRecvCallback onReceive;     // ���ջص�
    void *parent;                    // �ϲ�ģ��ָ��
};

// �ӿں�������
USARTInstance *USARTRegister(UART_HandleTypeDef *huart, USARTRecvCallback cb, void *parent);
bool USARTSendByte(USARTInstance *instance, uint8_t byte);
bool USARTSendArray(USARTInstance *instance, const uint8_t *data, uint16_t len);
void USART_IRQHandler_Callback(UART_HandleTypeDef *huart);

#endif
