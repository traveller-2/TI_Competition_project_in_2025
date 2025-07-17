#include "servo.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

#define SERVO_UART                &huart2  // 使用 USART2
#define SERVO_TX_BUF_LEN         64
#define SERVO_QUEUE_LEN          10

#define MAX_SERVO_NUM  2  // 最多支持2个舵机

volatile uint16_t servo_angle[MAX_SERVO_NUM] = {0};  // 存储角度值
volatile uint8_t  servo_status[MAX_SERVO_NUM] = {0}; // 0正常 1读取失败

osMutexId_t servoUartMutex;

/***********************************说明*********************************************
	*	uint16_t angle = servo_angle[1];  // 获取1号舵机角度
	*	if (servo_status[1] == 0)
	*	{
  * 	 // 正常读取
	*	}
	*	else
	*	{
  *	 // 读取失败（可能串口未连接、舵机没回应）
	*	}
	************************************************************************************/


//void ServoReadTask(void *argument)
//{
//    while (1)
//    {
//        for (uint8_t i = 0; i < MAX_SERVO_NUM; i++)
//        {
//            Servo_ReadAngleOnce(i);
//            osDelay(50);  // 多个舵机之间稍作延迟
//        }

//        osDelay(100); // 每 100ms 更新一次所有角度
//    }
//}

void Servo_ReadAngleOnce(uint8_t id)
{
    char tx_buf[20];
    char rx_buf[20];

    snprintf(tx_buf, sizeof(tx_buf), "#%03dPRAD!", id);
    
    osMutexAcquire(servoUartMutex, osWaitForever);  // 加锁

    HAL_UART_Transmit(SERVO_UART, (uint8_t *)tx_buf, strlen(tx_buf), HAL_MAX_DELAY);

    // 第一次接收：回显（丢弃）
    HAL_UART_Receive(SERVO_UART, (uint8_t *)rx_buf, sizeof(rx_buf), 100);

    // 第二次接收：真正回传数据
    if (HAL_UART_Receive(SERVO_UART, (uint8_t *)rx_buf, sizeof(rx_buf), 100) == HAL_OK)
    {
        int rx_id = 0, angle = 0;
        if (sscanf(rx_buf, "#%03dP%d!", &rx_id, &angle) == 2 && rx_id == id)
        {
            servo_angle[id] = angle;
            servo_status[id] = 0;
        }
        else
        {
            servo_status[id] = 1;
        }
    }
    else
    {
        servo_status[id] = 1;
    }

}




typedef struct {
    uint8_t id;
    uint16_t pwm;     // 500~2500
    uint16_t time_ms; // 0~9999
} ServoCmd_t;

static osMessageQueueId_t servoQueueHandle;
static char servo_tx_buf[SERVO_TX_BUF_LEN];

/***********************************说明*********************************************
	*Servo_Init();                    // 第一步：初始化
	*Servo_SetID(0, 1);               // 第二步（初次）：修改ID（每个舵机唯一）
	*Servo_SetMode(1, 1);             // 第三步：设置舵机模式（如270度）
	*
	* 控制多个舵机
	*Servo_QueueControl(1, 1500, 1000);
	*Servo_QueueControl(2, 2000, 1000);
	*
	* 可选：读取舵机角度
	*char buffer[20];
	*Servo_ReadAngle(1, buffer, 100);
	************************************************************************************/

// 初始化函数：创建队列 + 启动任务
void Servo_Init(void)
{
    servoQueueHandle = osMessageQueueNew(SERVO_QUEUE_LEN, sizeof(ServoCmd_t), NULL);
		servoUartMutex = osMutexNew(NULL);
}

// 舵机控制任务（串口发送线程）
void ServoControlTask(void *argument)
{
    ServoCmd_t cmd;
//		osDelay(500);
//		Servo_QueueControl(1, 1500, 1000);
//		osDelay(10);
//		Servo_QueueControl(2, 1500, 1000);
    while (1)
    {
        if (osMessageQueueGet(servoQueueHandle, &cmd, NULL, osWaitForever) == osOK)
        {
            snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
                     "#%03dP%04dT%04d!", cmd.id, cmd.pwm, cmd.time_ms);
						osMutexAcquire(servoUartMutex, osWaitForever);  //  加锁
						HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
						osMutexRelease(servoUartMutex);  //  解锁
        }
    }
}

// 外部调用接口：发送舵机控制命令
void Servo_QueueControl(uint8_t id, uint16_t pwm, uint16_t time_ms)
{
    ServoCmd_t cmd = {id, pwm, time_ms};
    osMessageQueuePut(servoQueueHandle, &cmd, 0, 0);

    // 同步更新角度值（模拟实际位置）
    servo_angle[id] = pwm;
    servo_status[id] = 0;
}

// 设置舵机 ID
void Servo_SetID(uint8_t old_id, uint8_t new_id)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPID%03d!", old_id, new_id);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}

// 设置舵机工作模式（1~8）
void Servo_SetMode(uint8_t id, uint8_t mode)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPMOD%d!", id, mode);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}

// 读取角度（需 TX-RX 短接）
void Servo_ReadAngle(uint8_t id, char *ret_buf, uint16_t timeout)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPRAD!", id);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
    HAL_UART_Receive(SERVO_UART, (uint8_t *)ret_buf, 16, timeout);
}

// 多舵机同步控制指令（你手动拼接后传入）
void Servo_MultiControl(const char *commands)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "{G0000%s}", commands);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}



/************************************************************************************************************
	*																					下面是PWM舵机																										*
	***********************************************************************************************************/
static uint16_t current_pulse = 1500; // 默认中间位置

void servo_init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
}

void servo_set_angle(uint8_t angle)
{
    if (angle > 180) angle = 180;
    current_pulse = 500 + ((uint32_t)angle * 2000) / 180; // 映射角度到脉宽
}

// 舵机 FreeRTOS 任务
//void ServoTask(void *argument)
//{
//    while (1)
//    {
//        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, current_pulse);  // 设置 PWM
//        osDelay(20); // 舵机刷新周期为 20ms
//    }
//}
