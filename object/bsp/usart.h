#ifndef _UART_UTILS_H_
#define _UART_UTILS_H_

#include <string.h>
#include <stdint.h>

// 数据缓冲区大小
#define UART_TX_BUFFER_SIZE 128

// 数据缓冲区变量声明（如果在 .c 文件中定义，则这里只写 extern）
extern uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
extern uint8_t uart_tx_len;

/**
 * @brief 发送一段字节数据，通过 UART 逐字节发送
 * @param data 数据指针
 * @param len  数据长度
 */
void UART_SendArray(uint8_t *data, uint8_t len);

#endif // _UART_UTILS_H_
