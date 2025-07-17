#include "gimbal_motor.h"    // 云台电机控制模块头文件
#include "bsp_usart.h"       // 串口底层发送封装
#include "cmsis_os.h"        // FreeRTOS接口
#include <string.h>
#include <stdbool.h>

// === 串口定义（可改为宏配置） ===
extern UART_HandleTypeDef huart1;           // 串口句柄（外部定义）
#define GIMBAL_UART  (&huart1)              // 云台电机控制使用的串口（例如 USART1）

// === 通用协议定义 ===
#define MOTOR_CHECK_CODE         0x6B        // 帧尾校验码，固定值，用于验证帧完整性

// === 功能码定义 ===
#define MOTOR_FUNC_VELOCITY_MODE  0xF6       // 电机速度控制功能码
#define MOTOR_FUNC_ORIGIN_MODE    0xE3       // 电机回原点控制功能码

#define MOTOR_SYNC_DISABLE        0x00       // 非同步控制
#define MOTOR_SYNC_ENABLE         0x01       // 启用同步控制（用于多电机归零等场景）

// === 消息队列对象句柄 ===
static osMessageQueueId_t gimbalCmdQueue = NULL;  // 云台控制命令队列句柄（静态私有）

// === 内部函数声明（私有函数） ===
static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir);   // 发送速度控制帧
static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync);                    // 发送归零控制帧
static void send_motor_frame(const uint8_t *data, uint8_t len);                        // 通用帧发送函数

/**
 * @brief 云台电机控制任务
 *        等待队列中的控制命令，封装协议帧并通过串口发送
 */
void GimbalMotor_Task(void *argument) {
    GimbalControlCmd_t cmd;  // 用于存放从消息队列取出的电机控制命令

    while (1) {
        // 检查消息队列是否创建成功，同时阻塞等待新的控制命令进入队列
        if (gimbalCmdQueue != NULL &&
            osMessageQueueGet(gimbalCmdQueue, &cmd, NULL, osWaitForever) == osOK) {

            /**
             * 根据命令中的 rpm 判断电机转动方向：
             * - 如果 rpm >= 0，则认为是“正转”，dir = 0
             * - 如果 rpm < 0，则认为是“反转”，dir = 1
             * 同时取 rpm 的绝对值作为转速
             */
            uint8_t dir = (cmd.rpm >= 0) ? 0 : 1;           // 方向标志（0：正转，1：反转）
            uint16_t rpm = (cmd.rpm >= 0) ? cmd.rpm : -cmd.rpm;  // 转速取绝对值

            // 调用内部函数封装并发送“速度控制协议帧”
            // 实际通过串口向电机下发控制指令
            send_motor_velocity(cmd.id, rpm, cmd.acc, dir);
        }
			osDelay(1);
        // 如果没有收到有效命令，会自动再次阻塞等待，无需延时
    }
}

void MyControlTask(void *argument) {
    GimbalControlCmd_t cmd;
		osDelay(1000);  			//等待电机上电
    
		// 让 1 号电机正向转动 300 rpm，加速度为 15
    cmd.id = 1;
    cmd.rpm = 40;
    cmd.acc = 5;
    GimbalMotor_SendCmd(&cmd);  //  发送控制命令

    osDelay(1000);  // 等一秒

    // 让 2 号电机反向转动 200 rpm，加速度为 10
    cmd.id = 1;
    cmd.rpm = -20;  // 负值代表反转
    cmd.acc = 5;
    GimbalMotor_SendCmd(&cmd);  // ? 再次发送
		for(;;)
		{
			osDelay(1);
		}
}


/**
 * @brief 初始化云台控制命令队列
 *        应在系统初始化阶段调用
 */
void GimbalMotor_QueueInit(void) {
    gimbalCmdQueue = osMessageQueueNew(
        GIMBAL_CMD_QUEUE_LEN,             // 队列长度（可定义为宏）
        sizeof(GimbalControlCmd_t),       // 每个元素大小
        NULL                              // 属性为默认
    );
}

/**
 * @brief 向云台电机控制任务发送一个控制命令
 * @param cmd 指向命令结构体的指针
 * @return true 成功加入队列，false 失败
 */
bool GimbalMotor_SendCmd(GimbalControlCmd_t *cmd) {
    if (gimbalCmdQueue == NULL || cmd == NULL)
        return false;

    return osMessageQueuePut(gimbalCmdQueue, cmd, 0, 0) == osOK;
}

/**
 * @brief 封装速度控制协议帧并发送
 * @param id  电机ID
 * @param rpm 电机转速（绝对值）
 * @param acc 加速度
 * @param dir 转动方向（0正转，1反转）
 */
static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir) {
    uint8_t frame[8] = {
        id,                             // 电机ID
        MOTOR_FUNC_VELOCITY_MODE,      // 功能码：速度模式
        dir,                            // 方向
        (uint8_t)(rpm >> 8),            // 高8位
        (uint8_t)(rpm & 0xFF),          // 低8位
        acc,                            // 加速度
        MOTOR_SYNC_DISABLE,             // 非同步模式
        MOTOR_CHECK_CODE                // 固定校验码
    };

    send_motor_frame(frame, sizeof(frame));
}

/**
 * @brief 封装归零控制协议帧并发送
 * @param id   电机ID
 * @param rpm  回零速度
 * @param sync 是否启用多机同步
 */
static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync) {
    uint8_t frame[7] = {
        id,                             // 电机ID
        MOTOR_FUNC_ORIGIN_MODE,        // 功能码：归零
        0x00,                           // 保留位（可扩展）
        (uint8_t)(rpm >> 8),            // 高8位速度
        (uint8_t)(rpm & 0xFF),          // 低8位速度
        sync ? MOTOR_SYNC_ENABLE : MOTOR_SYNC_DISABLE,  // 同步标志
        MOTOR_CHECK_CODE                // 校验码
    };

    send_motor_frame(frame, sizeof(frame));
}

/**
 * @brief 串口数据逐字节发送函数
 * @param data 数据帧指针
 * @param len  数据帧长度
 */
static void send_motor_frame(const uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; ++i) {
        BSP_USART_SendByte(GIMBAL_UART, data[i]);  // 使用 BSP 层封装的发送接口
    }
}
