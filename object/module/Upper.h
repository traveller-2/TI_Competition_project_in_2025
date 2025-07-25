#ifndef __UPPER_H
#define __UPPER_H

#include <stdint.h>
#include <stdbool.h>

void UART_RX_ParseByte(uint8_t byte);  // �����жϵ��ô˺�������ÿ���ֽ�

// ����֡ͷ2�����������ͬ float ����
extern volatile float value_type1;
extern volatile float value_type2;

// ��ʼ�� UART������ SysConfig ���ɵ�ȫ�ֳ�ʼ����
void UART_RX_Init(void);

// �Ƿ���յ����ֽ�
bool UART_HasNewByte(void);

void UART_UPPER_INST_IRQHandler(void);

// ��ȡ���յ����ֽڣ���ȡ���־�Զ������
uint8_t UART_ReadByte(void);


#endif
