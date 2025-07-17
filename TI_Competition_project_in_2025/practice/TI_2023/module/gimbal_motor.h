#ifndef __GIMBAL_MOTOR_H
#define __GIMBAL_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"  // 引入 FreeRTOS 消息队列类型

// === 用户可配置宏 ===
#define GIMBAL_CMD_QUEUE_LEN   8  // 队列长度可根据需要调整

// === 控制命令结构体 ===
typedef enum {
    MOTOR_CTRL_SPEED,
    MOTOR_CTRL_POSITION,
    MOTOR_CTRL_ZERO
} GimbalCtrlMode_t;

typedef struct {
    uint8_t id;        // 电机地址
    GimbalCtrlMode_t mode; // 控制模式
    int16_t rpm;       // 速度模式 or 位置模式转速
    uint8_t acc;       // 加速度档位
    int32_t pulse;     // 位置控制用：脉冲数
    uint8_t pos_mode;  // 位置控制用：相对0 / 绝对1
    uint8_t zero_mode; // 回零控制用：00/01/02/03
} GimbalControlCmd_t;



//typedef struct {
//    uint8_t  id;       // 电机 ID（通常为 1 或 2）
//    int16_t  rpm;      // 转速（正为正转，负为反转）
//    uint8_t  acc;      // 加速度（电机内部处理）
//} GimbalControlCmd_t;

// === API 函数声明 ===

/**
 * @brief 初始化云台电机控制的消息队列
 */
void GimbalMotor_QueueInit(void);

/**
 * @brief 云台电机任务主循环（FreeRTOS Task 用）
 * @param argument 未使用
 */
void GimbalMotor_Task(void *argument);

/**
 * @brief 发送控制命令到电机控制队列
 * @param cmd 控制命令指针
 * @return true 成功，false 失败
 */
bool GimbalMotor_SendCmd(GimbalControlCmd_t *cmd);

/**
 * @brief 立即触发一个回中命令（非队列方式）
 * @param id 电机 ID
 * @param rpm 回中转速
 * @param sync 是否启用同步模式
 */
void send_motor_velocity(uint8_t id, int16_t rpm, uint8_t acc, uint8_t dir);

void send_motor_position(uint8_t id, int16_t rpm, uint8_t acc, int32_t pulse, uint8_t pos_mode);

void send_motor_zeroing(uint8_t id, uint8_t zero_mode);

void send_motor_set_zero_position(uint8_t id, uint8_t save_flag);

void send_motor_frame(const uint8_t *data, uint8_t len);

#endif  // __GIMBAL_MOTOR_H
