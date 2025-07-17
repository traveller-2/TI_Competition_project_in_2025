#include "bsp_usart.h"
#include <string.h>

#define FRAME_LENGTH 7  // 帧长


void BSP_USART_Init(void) {
	
    // HAL_UART_Init() 已在 MX_USART1_UART_Init() 中完成
    // 如果你想手动初始化，这里可以写 HAL_UART_Init(&huart1);
}

void BSP_USART_SendByte(UART_HandleTypeDef *huart, uint8_t byte) {
    HAL_UART_Transmit(huart, &byte, 1, HAL_MAX_DELAY);
}

void BSP_USART_SendString(UART_HandleTypeDef *huart, const char *str) {
    while (*str) {
        BSP_USART_SendByte(huart, (uint8_t)(*str));
        str++;
    }
}

void BSP_USART_SendBuffer(UART_HandleTypeDef *huart, const uint8_t *buf, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        BSP_USART_SendByte(huart, buf[i]);
    }
}


