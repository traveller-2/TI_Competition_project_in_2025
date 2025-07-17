#ifndef __MOTOR_MONITOR_H__
#define __MOTOR_MONITOR_H__

#include "stdint.h"
#include "cmsis_os.h"

// 宏定义
#define PULSE_PER_TURN       44     // 每圈编码器脉冲数
#define GEAR_RATIO           30     // 减速比
#define MONITOR_INTERVAL_MS  200    // RPM 更新周期（ms）
#define dt                   (MONITOR_INTERVAL_MS / 1000.0f)  // 秒数间隔

// 全局变量（供外部 OLED 显示任务等使用）
extern int rpm[4];  // 当前计算得到的 RPM
extern volatile float speed;
extern int32_t delta;

// 任务函数声明
void MotorRPM_Update_Task(void *argument);  // 作为 FreeRTOS 任务入口

// 功能函数声明
void Motor_UpdateRPM(void);  // 实际的 RPM 计算逻辑

#endif  // __MOTOR_MONITOR_H__
