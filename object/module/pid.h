
#ifndef __PID_H
#define __PID_H

typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float setpoint;     // 目标值
    float integral;     // 积分累计
    float prev_error;   // 上一次误差
    float output;       // 输出结果

    float max_output;   // 输出限幅
    float integral_limit; // 积分限幅
} PID_t;

//extern PID_t pid_x_motor;    // 左电机 PID 控制器
//extern PID_t pid_y_motor;   // 右电机 PID 控制器

// 初始化 PID 参数
void PID_Init(PID_t *pid, float kp, float ki, float kd, float max_out, float integral_limit);

// 单次计算 PID 输出
float PID_Compute(PID_t *pid, float measured);

#endif
