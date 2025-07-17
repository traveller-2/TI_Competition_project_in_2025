#include "device_reader.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>

// ===== ȫ�ֱ��� =====
float delta_x = 0.0f;
float delta_y = 0.0f;

uint8_t uart_rx_byte;         // ����һ���ֽ�
uint8_t rx_buffer[FRAME_LENGTH];  // ���ջ���
uint8_t rx_index = 0;             // ��ǰ����λ��


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) // �ж���USART2
    {
        rx_buffer[rx_index++] = uart_rx_byte;

        // �жϽ�����7�ֽ�
        if (rx_index >= 7)
        {
            if (rx_buffer[0] == 0xAA && rx_buffer[6] == 0x55)
            {
                uint8_t type = rx_buffer[1];  // ֡ͷ2������
                float value;
                memcpy(&value, &rx_buffer[2], 4); // ���� float ����

                if (type == 0x01)
                {
                    delta_x = value;
                }
                else if (type == 0x02)
                {
                    delta_y = value;
                }
            }

            rx_index = 0; // ���ý���
        }

        // ����������һ���ֽ�
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}
