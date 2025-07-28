#include "pid.h"


void PID_Init(PID_t *pid, float kp, float ki, float kd, float max_out, float integral_limit)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;

    pid->setpoint = 0;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->output = 0;

    pid->max_output = max_out;
    pid->integral_limit = integral_limit;
}

float PID_Compute(PID_t *pid, float measured)
{
    float error = pid->setpoint - measured;

    pid->integral += error;
    // ���ƻ����������ֿ�������
    if (pid->integral > pid->integral_limit)
        pid->integral = pid->integral_limit;
    else if (pid->integral < -pid->integral_limit)
        pid->integral = -pid->integral_limit;

    float derivative = error - pid->prev_error;

    pid->output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // ����޷�
    if (pid->output > pid->max_output)
        pid->output = pid->max_output;
    else if (pid->output < -pid->max_output)
        pid->output = -pid->max_output;

    pid->prev_error = error;

    return pid->output;
}

