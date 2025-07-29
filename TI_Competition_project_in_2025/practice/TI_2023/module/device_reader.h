#ifndef __DEVICE_READER_H
#define __DEVICE_READER_H

#include <stdint.h>
#include "usart.h"

// ===== �궨�岿�� =====
#define FRAME_HEAD_1   0xAA
#define FRAME_HEAD_2_X 0x01  // delta_x
#define FRAME_HEAD_2_Y 0x02  // delta_y
#define FRAME_TAIL     0x55

#define FRAME_LENGTH   6


// ��������������������ģ���ж�ȡ��ǰλ����Ϣ
extern float delta_x;
extern float delta_y;

extern uint8_t uart_rx_byte;         // ����һ���ֽ�

void DeviceReader_Init(void);

#endif
