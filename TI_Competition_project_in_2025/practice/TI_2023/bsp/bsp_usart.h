#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f4xx_hal.h"
#include "usart.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"  // 为了使用 memcpy


void BSP_USART_Init(void);
void BSP_USART_SendByte(UART_HandleTypeDef *huart, uint8_t byte);
void BSP_USART_SendString(UART_HandleTypeDef *huart, const char *str);
void BSP_USART_SendBuffer(UART_HandleTypeDef *huart, const uint8_t *buf, uint16_t len);


#endif
