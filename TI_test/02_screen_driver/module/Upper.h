#ifndef __UPPER_H
#define __UPPER_H

#include <stdint.h>
#include <stdbool.h>

void UART_RX_ParseByte(uint8_t byte);  // 串口中断调用此函数处理每个字节

// 根据帧头2种类解析到不同 float 变量
extern volatile float value_type1;
extern volatile float value_type2;

// 初始化 UART（调用 SysConfig 生成的全局初始化）
void UART_RX_Init(void);

// 是否接收到新字节
bool UART_HasNewByte(void);

void UART_UPPER_INST_IRQHandler(void);

// 读取接收到的字节（读取后标志自动清除）
uint8_t UART_ReadByte(void);


#endif
