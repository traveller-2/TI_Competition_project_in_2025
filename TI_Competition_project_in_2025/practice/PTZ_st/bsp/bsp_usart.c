#include "bsp_usart.h"
#include <string.h>

#define USART_MAX_INSTANCE 5  // ֧�����5������ʵ��

// �洢����ע��Ĵ���ʵ��
static USARTInstance usart_instances[USART_MAX_INSTANCE];

// ��ע��ʵ������
static int usart_instance_count = 0;

/**
 * @brief ע��һ�� USART ʵ����֧�ֻص����ϲ����ָ��󶨣�
 * @param huart HAL UART ���
 * @param cb ���ջص����������յ� 1 �ֽں���ã�
 * @param parent �ϲ�ģ��ָ�루��ѡ���� motor ʵ����
 * @return ע��ɹ���ʵ��ָ�룻ʧ�ܷ��� NULL
 */
USARTInstance *USARTRegister(UART_HandleTypeDef *huart, USARTRecvCallback cb, void *parent) {
    if (usart_instance_count >= USART_MAX_INSTANCE) return NULL;

    USARTInstance *inst = &usart_instances[usart_instance_count++];
    inst->huart = huart;
    inst->onReceive = cb;
    inst->parent = parent;

    // ���������жϣ��״ν���1�ֽڣ�ռλ������
    HAL_UART_Receive_IT(huart, (uint8_t *)&(uint8_t){0}, 1);

    return inst;
}

/**
 * @brief ����һ���ֽ�
 * @param instance USART ʵ��
 * @param byte �����͵��ֽ�
 * @return �Ƿ��ͳɹ�
 */
bool USARTSendByte(USARTInstance *instance, uint8_t byte) {
    return HAL_UART_Transmit(instance->huart, &byte, 1, 10) == HAL_OK;
}

/**
 * @brief ���Ͷ���ֽ�
 * @param instance USART ʵ��
 * @param data ��������ָ��
 * @param len ���ݳ���
 * @return �Ƿ��ͳɹ�
 */
bool USARTSendArray(USARTInstance *instance, const uint8_t *data, uint16_t len) {
    return HAL_UART_Transmit(instance->huart, (uint8_t *)data, len, 100) == HAL_OK;
}

/**
 * @brief HAL_UART_IRQHandler ��ɺ���õĻص������ڿ���еĽ��մ���
 * @param huart ��Ӧ�� HAL UART ���
 */
void USART_IRQHandler_Callback(UART_HandleTypeDef *huart) {
    for (int i = 0; i < usart_instance_count; ++i) {
        if (usart_instances[i].huart == huart) {
            uint8_t data;

            // ������ȡ�ս��յ����ֽڣ�ע�ⲻ��ʡ�ԣ�
            HAL_UART_Receive(huart, &data, 1, 1);

            // �����ע��ص��������û������߼�
            if (usart_instances[i].onReceive)
                usart_instances[i].onReceive(&usart_instances[i], data);

            // ������һ�ν����жϣ�����1�ֽڣ�
            HAL_UART_Receive_IT(huart, (uint8_t *)&(uint8_t){0}, 1);
            return;
        }
    }
}