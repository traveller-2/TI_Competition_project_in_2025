#ifndef __GIMBAL_MOTOR_H
#define __GIMBAL_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"  // ���� FreeRTOS ��Ϣ��������

// === �û������ú� ===
#define GIMBAL_CMD_QUEUE_LEN   8  // ���г��ȿɸ�����Ҫ����

// === ��������ṹ�� ===
typedef enum {
    MOTOR_CTRL_SPEED,
    MOTOR_CTRL_POSITION,
    MOTOR_CTRL_ZERO
} GimbalCtrlMode_t;

typedef struct {
    uint8_t id;        // �����ַ
    GimbalCtrlMode_t mode; // ����ģʽ
    int16_t rpm;       // �ٶ�ģʽ or λ��ģʽת��
    uint8_t acc;       // ���ٶȵ�λ
    int32_t pulse;     // λ�ÿ����ã�������
    uint8_t pos_mode;  // λ�ÿ����ã����0 / ����1
    uint8_t zero_mode; // ��������ã�00/01/02/03
} GimbalControlCmd_t;



//typedef struct {
//    uint8_t  id;       // ��� ID��ͨ��Ϊ 1 �� 2��
//    int16_t  rpm;      // ת�٣���Ϊ��ת����Ϊ��ת��
//    uint8_t  acc;      // ���ٶȣ�����ڲ�����
//} GimbalControlCmd_t;

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
void send_motor_velocity(uint8_t id, int16_t rpm, uint8_t acc, uint8_t dir);

void send_motor_position(uint8_t id, int16_t rpm, uint8_t acc, int32_t pulse, uint8_t pos_mode);

void send_motor_zeroing(uint8_t id, uint8_t zero_mode);

void send_motor_set_zero_position(uint8_t id, uint8_t save_flag);

void send_motor_frame(const uint8_t *data, uint8_t len);

#endif  // __GIMBAL_MOTOR_H
