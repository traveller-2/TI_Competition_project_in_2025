#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stdint.h"

// �Ҷȵ�ƽ����
#define GRAY_BLACK 0  // ����
#define GRAY_WHITE 1  // �׵�
#define ACC_STEP 1             // ÿ�ε�����ݼ�1

#define CONTROL_INTERVAL 20    // ��������Ϊ 20ms
#define ACCEL_INTERVAL 60      // ÿ 60ms ִ��һ�μ��٣��ɵ���
#define NUM_SENSORS 5


typedef struct {
  float target;      // Ŀ��ֵ
  float current;     // ��ǰֵ
  float Kp, Ki, Kd; // PID����
  float integral;    // ������
  float last_error;  // �ϴ����
} PID_Controller;

// ��ʼ������ѭ������
void ChassisFollowTask_Init(void);
float Calculate_Error(void);
float PID_Update(PID_Controller* pid, float current);
void Motor_Control(float output);
// ���������
void ChassisFollowTask(void *argument);
static void Read_All_GraySensors(uint8_t *gray);
extern float posError;
extern int flag;
float Distance_PID_Update(PID_Controller* pid, float current_mm);

#endif
