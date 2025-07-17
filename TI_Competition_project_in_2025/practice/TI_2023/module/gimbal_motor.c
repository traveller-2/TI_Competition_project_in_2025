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
//static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir);   // 发送速度控制帧
//static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync);                    // 发送归零控制帧
//static void send_motor_frame(const uint8_t *data, uint8_t len);                        // 通用帧发送函数

/**
 * @brief 云台电机控制任务
 *        等待队列中的控制命令，封装协议帧并通过串口发送
 */
void GimbalMotor_Task(void *argument) {
    GimbalControlCmd_t cmd;

    while (1) {
        if (gimbalCmdQueue != NULL &&
            osMessageQueueGet(gimbalCmdQueue, &cmd, NULL, osWaitForever) == osOK) {
            
            switch (cmd.mode) {
                case MOTOR_CTRL_SPEED:
                    send_motor_velocity(cmd.id, cmd.rpm, cmd.acc, (cmd.rpm >= 0) ? 0 : 1);
                    break;

                case MOTOR_CTRL_POSITION:
                    send_motor_position(cmd.id, cmd.rpm, cmd.acc, cmd.pulse, cmd.pos_mode);
                    break;

                case MOTOR_CTRL_ZERO:
                    send_motor_zeroing(cmd.id, cmd.zero_mode);
                    break;
            }
        }

        osDelay(1);  // 为其他任务让出时间片
    }
}



void MyControlTask(void *argument) {
    GimbalControlCmd_t cmd;
		// 先人为旋转电机到希望作为“零点”的位置
		// 然后执行下面的命令设置此位置为零点（并保存进闪存）
		osDelay(1500);
		send_motor_set_zero_position(1, 1);  // 对 ID=1 的电机，设置当前位置为零点并保存

    osDelay(500);

    //速度模式控制：正转200 RPM，10加速度档
		memset(&cmd, 0, sizeof(cmd));  // 清除所有字段
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_SPEED;
    cmd.rpm = 200;
    cmd.acc = 10;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(5000);
	
	  //速度模式控制：正转 0 RPM，10加速度档
		memset(&cmd, 0, sizeof(cmd));  // 清除所有字段
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_SPEED;
    cmd.rpm = 0;
    cmd.acc = 10;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(5000);


    //位置模式控制：正转10圈（32000脉冲），200RPM，无加速度曲线，相对模式
		memset(&cmd, 0, sizeof(cmd));  // 清除所有字段
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_POSITION;
    cmd.rpm = 200;
    cmd.acc = 10;
    cmd.pulse = 32000;
    cmd.pos_mode = 0;  // 0 相对位置模式，1 绝对位置模式
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);
		
		osDelay(10000);
		
		 //回零模式控制：单圈就近回零
		memset(&cmd, 0, sizeof(cmd));  // 清除所有字段
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_ZERO;
    cmd.zero_mode = 0;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(1000);

    while (1) {
        osDelay(1000); // 防止空跑
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
void send_motor_velocity(uint8_t id, int16_t rpm, uint8_t acc, uint8_t dir) {
    uint8_t buf[8];
    buf[0] = id;
    buf[1] = 0xF6;
    buf[2] = dir;
    buf[3] = (uint8_t)((rpm >> 8) & 0xFF);
    buf[4] = (uint8_t)(rpm & 0xFF);
    buf[5] = acc;
    buf[6] = 0x00; // 不启用多机同步
    buf[7] = 0x6B; // 校验字节（如无算法可写死）
//    HAL_UART_Transmit(&huart2, buf, 8, 100);
		send_motor_frame(buf, sizeof(buf));
}

void send_motor_position(uint8_t id, int16_t rpm, uint8_t acc, int32_t pulse, uint8_t pos_mode) {
    uint8_t buf[13];
    buf[0] = id;
    buf[1] = 0xFD;
    buf[2] = (rpm >= 0) ? 0 : 1;
    buf[3] = (uint8_t)((rpm >> 8) & 0xFF);
    buf[4] = (uint8_t)(rpm & 0xFF);
    buf[5] = acc;
    buf[6] = (pulse >> 24) & 0xFF;
    buf[7] = (pulse >> 16) & 0xFF;
    buf[8] = (pulse >> 8) & 0xFF;
    buf[9] = pulse & 0xFF;
    buf[10] = pos_mode;
    buf[11] = 0x00; // 不启用多机同步
    buf[12] = 0x6B;
//    HAL_UART_Transmit(&huart2, buf, 13, 100);
		send_motor_frame(buf, sizeof(buf));
}

void send_motor_zeroing(uint8_t id, uint8_t zero_mode) {
    uint8_t buf[5];
    buf[0] = id;
    buf[1] = 0x9A;
    buf[2] = zero_mode;
    buf[3] = 0x00;
    buf[4] = 0x6B;
//    HAL_UART_Transmit(&huart2, buf, 5, 100);
		send_motor_frame(buf, sizeof(buf));
}

void send_motor_set_zero_position(uint8_t id, uint8_t save_flag) {
    uint8_t buf[5];
    buf[0] = id;          // 电机 ID
    buf[1] = 0x93;        // 指令类型：设置零点
    buf[2] = 0x88;        // 固定子命令码
    buf[3] = save_flag;   // 是否存储：0x01 为存储到闪存
    buf[4] = 0x6B;        // 固定校验字节

    send_motor_frame(buf, sizeof(buf));
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
