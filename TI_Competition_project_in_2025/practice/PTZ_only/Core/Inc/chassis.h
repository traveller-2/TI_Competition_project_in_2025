#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stdint.h"

// 灰度电平定义
#define GRAY_BLACK 0  // 黑线
#define GRAY_WHITE 1  // 白底
#define ACC_STEP 1             // 每次递增或递减1

#define CONTROL_INTERVAL 20    // 控制周期为 20ms
#define ACCEL_INTERVAL 60      // 每 60ms 执行一次加速（可调）
#define NUM_SENSORS 5


typedef struct {
  float target;      // 目标值
  float current;     // 当前值
  float Kp, Ki, Kd; // PID参数
  float integral;    // 积分项
  float last_error;  // 上次误差
} PID_Controller;

// 初始化底盘循迹任务
void ChassisFollowTask_Init(void);
float Calculate_Error(void);
float PID_Update(PID_Controller* pid, float current);
void Motor_Control(float output);
// 任务函数入口
void ChassisFollowTask(void *argument);
static void Read_All_GraySensors(uint8_t *gray);
extern float posError;
extern int flag;
float Distance_PID_Update(PID_Controller* pid, float current_mm);

#endif
