#ifndef __MOTOR_MONITOR_H__
#define __MOTOR_MONITOR_H__

#include "stdint.h"
#include "cmsis_os.h"

// �궨��
#define PULSE_PER_TURN       44     // ÿȦ������������
#define GEAR_RATIO           30     // ���ٱ�
#define MONITOR_INTERVAL_MS  200    // RPM �������ڣ�ms��
#define dt                   (MONITOR_INTERVAL_MS / 1000.0f)  // �������

// ȫ�ֱ��������ⲿ OLED ��ʾ�����ʹ�ã�
extern int rpm[4];  // ��ǰ����õ��� RPM
extern volatile float speed;
extern int32_t delta;

// ����������
void MotorRPM_Update_Task(void *argument);  // ��Ϊ FreeRTOS �������

// ���ܺ�������
void Motor_UpdateRPM(void);  // ʵ�ʵ� RPM �����߼�

#endif  // __MOTOR_MONITOR_H__
