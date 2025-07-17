#ifndef __SERVO_H
#define __SERVO_H

#include "main.h"
#include "tim.h"
#include "stdint.h"

#define MAX_SERVO_NUM  2  // 最多支持2个舵机
extern volatile uint16_t servo_angle[MAX_SERVO_NUM] ;

void Servo_ReadAngleOnce(uint8_t id);//舵机角度读取


/**
 * 舵机初始化函数
 * 用于初始化队列和创建控制任务，必须在系统启动后调用一次
 */
void Servo_Init(void);



/**
 * 舵机控制任务（内部使用）
 * 不需要用户调用，由 Servo_Init 自动创建
 */
void ServoControlTask(void *argument);



/**
 * 队列式舵机控制函数（推荐使用）
 * 将控制命令放入消息队列，由后台任务顺序执行，避免串口冲突
 *
 * @param id       舵机 ID（范围 0~254）
 * @param pwm      伪 PWM 值（范围 500~2500，实际为目标位置）
 * @param time_ms  动作完成时间（单位 ms，范围 0~9999）
 */
void Servo_QueueControl(uint8_t id, uint16_t pwm, uint16_t time_ms);



/**
 * 设置舵机 ID
 * @param old_id   原舵机 ID
 * @param new_id   新 ID，必须唯一
 * ?? 修改 ID 时请只连接一个舵机，不能串联
 */
void Servo_SetID(uint8_t old_id, uint8_t new_id);



/**
 * 设置舵机工作模式（1~8）
 * 具体模式含义见说明书，如 1=270度顺时针，5=定圈顺时针等
 */
void Servo_SetMode(uint8_t id, uint8_t mode);



/**
 * 读取舵机当前角度值（伪PWM值），返回格式如：#001P1500!
 * ?? 需要 TXD 与 RXD 短接才能收到返回值
 */
void Servo_ReadAngle(uint8_t id, char *ret_buf, uint16_t timeout);



/**
 * 多舵机组合控制
 * 参数格式为拼接的多条命令（不带 {}），本函数会自动加上 {G0000 ... }
 * 示例：
 *   Servo_MultiControl("#000P1500T1000!#001P2500T0500!");
 */
void Servo_MultiControl(const char *commands);

////////////////////////////////////////////////////////下边是PWM舵机

// 初始化 PWM
void servo_init(void);

// 设置角度（0~180°）
void servo_set_angle(uint8_t angle);

// 舵机控制任务
void ServoTask(void *argument);

#endif
