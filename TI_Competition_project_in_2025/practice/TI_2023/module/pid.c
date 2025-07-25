#include "pid.h"

PID_t pid_x_motor;    // 左电机 PID 控制器
PID_t pid_y_motor;   // 右电机 PID 控制器

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
    // 限制积分项避免积分快速扩大
    if (pid->integral > pid->integral_limit)
        pid->integral = pid->integral_limit;
    else if (pid->integral < -pid->integral_limit)
        pid->integral = -pid->integral_limit;

    float derivative = error - pid->prev_error;

    pid->output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // 输出限幅
    if (pid->output > pid->max_output)
        pid->output = pid->max_output;
    else if (pid->output < -pid->max_output)
        pid->output = -pid->max_output;

    pid->prev_error = error;

    return pid->output;
}

