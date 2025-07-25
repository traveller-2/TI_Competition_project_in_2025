
#include "ti_msp_dl_config.h"
#include "usart.h"
#include <string.h>
#include <stdint.h>


// 数据缓冲区
uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE] = {0};

// 实际发送数据长度
uint8_t uart_tx_len = 0;

/**
 * @brief 将字符串或字节数组存入缓冲区，并逐字节通过 UART 发送
 * @param data 要发送的数据指针
 * @param len  要发送的数据长度
 */
void UART_SendArray(uint8_t *data, uint8_t len)
{
    if (len > UART_TX_BUFFER_SIZE) len = UART_TX_BUFFER_SIZE;

    // 存入数组
    for (uint8_t i = 0; i < len; i++) {
        uart_tx_buffer[i] = data[i];
    }

    uart_tx_len = len;

    // 逐字节发送
    for (uint8_t i = 0; i < uart_tx_len; i++) {
        DL_UART_transmitDataBlocking(UART_Emm_INST, uart_tx_buffer[i]);
    }
}