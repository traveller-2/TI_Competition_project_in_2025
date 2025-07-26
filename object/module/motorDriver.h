#ifndef __MOTOR_DRIVER_H__
#define __MOTOR_DRIVER_H__

#include <stdint.h>

// 电机速度设置函数
// 参数范围：-100 ~ 100（正数正转，负数反转）
void Motor_SetSpeed(int8_t left, int8_t right);

#endif // __MOTOR_DRIVER_H__
