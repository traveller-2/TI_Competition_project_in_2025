// pid.h

#ifndef __PID_H
#define __PID_H

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float setpoint;     // Ŀ��ֵ
    float integral;     // �����ۼ�
    float prev_error;   // ��һ�����
    float output;       // ������

    float max_output;   // ����޷�
    float integral_limit; // �����޷�
} PID_t;

extern PID_t pid_x_motor;    // ���� PID ������
extern PID_t pid_y_motor;   // �ҵ�� PID ������

// ��ʼ�� PID ����
void PID_Init(PID_t *pid, float kp, float ki, float kd, float max_out, float integral_limit);

// ���μ��� PID ���
float PID_Compute(PID_t *pid, float measured);

#endif
