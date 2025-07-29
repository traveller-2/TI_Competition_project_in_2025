#include "device_reader.h"
#include "bsp_usart.h"
#include <string.h>
#include <stdio.h>
#include "coord_queue.h"

// ===== 全局变量 =====
float delta_x = 0.0f;
float delta_y = 0.0f;
int cnt = 0;

float num[100] = {0};

uint8_t uart_rx_byte;         // 接收一个字节
uint8_t rx_buffer[FRAME_LENGTH];  // 接收缓存
uint8_t rx_index = 0;             // 当前接收位置


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) // 判断是USART2
    {
        rx_buffer[rx_index++] = uart_rx_byte;

        // 判断接收满7字节
        if (rx_index >= 6)
        {
            if (rx_buffer[0] == 0xA5 && rx_buffer[5] == 0x5A)
            {
                uint8_t type = rx_buffer[1];  // 帧头2：类型
                float value;
                memcpy(&value, &rx_buffer[1], 4); // 解析 float 数据
								num[cnt] = value;
								enqueue_coord(&test,value,(float)cnt++);
								
//                if (type == 0x01)
//                {
//                    delta_x = value;
//                }
//                else if (type == 0x02)
//                {
//                    delta_y = value;
//                }
            }

            rx_index = 0; // 重置接收
        }

        // 继续接收下一个字节
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}
