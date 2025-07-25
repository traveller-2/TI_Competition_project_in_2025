#ifndef __HW_UART_H
#define __HW_UART_H

#include "ti_msp_dl_config.h"



void uart0_send_char(char ch); //串口0发送单个字符
void uart0_send_string(char* str); //串口0发送字符串

#endif
