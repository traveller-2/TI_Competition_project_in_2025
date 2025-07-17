#include "bsp_usart.h"
#include <string.h>

#define USART_MAX_INSTANCE 5  // 支持最多5个串口实例

// 存储所有注册的串口实例
static USARTInstance usart_instances[USART_MAX_INSTANCE];

// 已注册实例数量
static int usart_instance_count = 0;

/**
 * @brief 注册一个 USART 实例（支持回调与上层对象指针绑定）
 * @param huart HAL UART 句柄
 * @param cb 接收回调函数（接收到 1 字节后调用）
 * @param parent 上层模块指针（可选，如 motor 实例）
 * @return 注册成功的实例指针；失败返回 NULL
 */
USARTInstance *USARTRegister(UART_HandleTypeDef *huart, USARTRecvCallback cb, void *parent) {
    if (usart_instance_count >= USART_MAX_INSTANCE) return NULL;

    USARTInstance *inst = &usart_instances[usart_instance_count++];
    inst->huart = huart;
    inst->onReceive = cb;
    inst->parent = parent;

    // 启动接收中断，首次接收1字节（占位变量）
    HAL_UART_Receive_IT(huart, (uint8_t *)&(uint8_t){0}, 1);

    return inst;
}

/**
 * @brief 发送一个字节
 * @param instance USART 实例
 * @param byte 待发送的字节
 * @return 是否发送成功
 */
bool USARTSendByte(USARTInstance *instance, uint8_t byte) {
    return HAL_UART_Transmit(instance->huart, &byte, 1, 10) == HAL_OK;
}

/**
 * @brief 发送多个字节
 * @param instance USART 实例
 * @param data 数据数组指针
 * @param len 数据长度
 * @return 是否发送成功
 */
bool USARTSendArray(USARTInstance *instance, const uint8_t *data, uint16_t len) {
    return HAL_UART_Transmit(instance->huart, (uint8_t *)data, len, 100) == HAL_OK;
}

/**
 * @brief HAL_UART_IRQHandler 完成后调用的回调，用于框架中的接收处理
 * @param huart 对应的 HAL UART 句柄
 */
void USART_IRQHandler_Callback(UART_HandleTypeDef *huart) {
    for (int i = 0; i < usart_instance_count; ++i) {
        if (usart_instances[i].huart == huart) {
            uint8_t data;

            // 阻塞读取刚接收到的字节（注意不能省略）
            HAL_UART_Receive(huart, &data, 1, 1);

            // 如果已注册回调，触发用户处理逻辑
            if (usart_instances[i].onReceive)
                usart_instances[i].onReceive(&usart_instances[i], data);

            // 继续下一次接收中断（接收1字节）
            HAL_UART_Receive_IT(huart, (uint8_t *)&(uint8_t){0}, 1);
            return;
        }
    }
}