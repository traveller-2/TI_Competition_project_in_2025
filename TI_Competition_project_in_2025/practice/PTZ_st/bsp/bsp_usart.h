#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "usart.h"
#include <stdint.h>
#include <stdbool.h>

// 前向声明结构体
struct __USARTInstance;
typedef struct __USARTInstance USARTInstance;

// 接收回调函数指针类型
typedef void (*USARTRecvCallback)(USARTInstance *inst, uint8_t data);

// USART 实例结构体
struct __USARTInstance {
    UART_HandleTypeDef *huart;       // HAL串口句柄
    USARTRecvCallback onReceive;     // 接收回调
    void *parent;                    // 上层模块指针
};

// 接口函数声明
USARTInstance *USARTRegister(UART_HandleTypeDef *huart, USARTRecvCallback cb, void *parent);
bool USARTSendByte(USARTInstance *instance, uint8_t byte);
bool USARTSendArray(USARTInstance *instance, const uint8_t *data, uint16_t len);
void USART_IRQHandler_Callback(UART_HandleTypeDef *huart);

#endif
