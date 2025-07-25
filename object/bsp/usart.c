
#include "ti_msp_dl_config.h"
#include "usart.h"
#include <string.h>
#include <stdint.h>


// ���ݻ�����
uint8_t uart_tx_buffer[UART_TX_BUFFER_SIZE] = {0};

// ʵ�ʷ������ݳ���
uint8_t uart_tx_len = 0;

/**
 * @brief ���ַ������ֽ�������뻺�����������ֽ�ͨ�� UART ����
 * @param data Ҫ���͵�����ָ��
 * @param len  Ҫ���͵����ݳ���
 */
void UART_SendArray(uint8_t *data, uint8_t len)
{
    if (len > UART_TX_BUFFER_SIZE) len = UART_TX_BUFFER_SIZE;

    // ��������
    for (uint8_t i = 0; i < len; i++) {
        uart_tx_buffer[i] = data[i];
    }

    uart_tx_len = len;

    // ���ֽڷ���
    for (uint8_t i = 0; i < uart_tx_len; i++) {
        DL_UART_transmitDataBlocking(UART_Emm_INST, uart_tx_buffer[i]);
    }
}