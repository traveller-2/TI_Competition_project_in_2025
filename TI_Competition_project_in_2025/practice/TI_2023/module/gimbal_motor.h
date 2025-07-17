#ifndef __GIMBAL_MOTOR_H
#define __GIMBAL_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"  // ���� FreeRTOS ��Ϣ��������

// === �û������ú� ===
#define GIMBAL_CMD_QUEUE_LEN   8  // ���г��ȿɸ�����Ҫ����

// === ��������ṹ�� ===
typedef struct {
    uint8_t  id;       // ��� ID��ͨ��Ϊ 1 �� 2��
    int16_t  rpm;      // ת�٣���Ϊ��ת����Ϊ��ת��
    uint8_t  acc;      // ���ٶȣ�����ڲ�����
} GimbalControlCmd_t;

// === API �������� ===

/**
 * @brief ��ʼ����̨������Ƶ���Ϣ����
 */
void GimbalMotor_QueueInit(void);

/**
 * @brief ��̨���������ѭ����FreeRTOS Task �ã�
 * @param argument δʹ��
 */
void GimbalMotor_Task(void *argument);

/**
 * @brief ���Ϳ������������ƶ���
 * @param cmd ��������ָ��
 * @return true �ɹ���false ʧ��
 */
bool GimbalMotor_SendCmd(GimbalControlCmd_t *cmd);

/**
 * @brief ��������һ����������Ƕ��з�ʽ��
 * @param id ��� ID
 * @param rpm ����ת��
 * @param sync �Ƿ�����ͬ��ģʽ
 */
void send_motor_origin(uint8_t id, uint16_t rpm, bool sync);

void send_motor_frame(const uint8_t *data, uint8_t len);

#endif  // __GIMBAL_MOTOR_H
