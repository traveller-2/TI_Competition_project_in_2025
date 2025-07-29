#ifndef __DEVICE_READER_H
#define __DEVICE_READER_H

#include <stdint.h>
#include "usart.h"

// ===== 宏定义部分 =====
#define FRAME_HEAD_1   0xAA
#define FRAME_HEAD_2_X 0x01  // delta_x
#define FRAME_HEAD_2_Y 0x02  // delta_y
#define FRAME_TAIL     0x55

#define FRAME_LENGTH   6


// 公共变量：用于在其他模块中读取当前位移信息
extern float delta_x;
extern float delta_y;

extern uint8_t uart_rx_byte;         // 接收一个字节

void DeviceReader_Init(void);

#endif
