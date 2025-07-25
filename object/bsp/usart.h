#ifndef _UART_UTILS_H_
#define _UART_UTILS_H_

#include <string.h>
#include <stdint.h>

// ���ݻ�������С
#define UART_TX_BUFFER_SIZE 128

// ���ݻ�������������������� .c �ļ��ж��壬������ֻд extern��
extern uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE];
extern uint8_t uart_tx_len;

/**
 * @brief ����һ���ֽ����ݣ�ͨ�� UART ���ֽڷ���
 * @param data ����ָ��
 * @param len  ���ݳ���
 */
void UART_SendArray(uint8_t *data, uint8_t len);

#endif // _UART_UTILS_H_
