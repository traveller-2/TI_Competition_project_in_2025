#include "soft_iic.h"
#include "motor_iic.h"  // 自己建的 motor_iic.h，声明函数
#include "cmsis_os.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "semphr.h"

#define MOTOR_I2C_ADDR 0x34
#define CAM_DEFAULT_I2C_ADDRESS       (0x34)			//I2C地址
#define MOTOR_TYPE_ADDR               0x14 				//编码电机类型设置寄存器地址
#define MOTOR_FIXED_SPEED_ADDR       	0x33				//速度寄存器地址。属于闭环控制
#define MOTOR_ENCODER_POLARITY_ADDR   0x15				//电机编码方向极性地址
#define MOTOR_FIXED_PWM_ADDR      		0x1F				//固定PWM控制地址，属于开环控制
#define MOTOR_ENCODER_TOTAL_ADDR  		0x3C				//4个编码电机各自的总脉冲值
#define ADC_BAT_ADDR                  0x00				//电压地址

#define MOTOR_TYPE_WITHOUT_ENCODER        0
#define MOTOR_TYPE_TT                     1
#define MOTOR_TYPE_N20                    2
#define MOTOR_TYPE_JGB37_520_12V_110RPM   3

typedef struct {
    int8_t speed[4];
} MotorSpeedMsg_t;


int8_t speedForward[4] = {100, 50, 50, 50};
int8_t speedBackward[4] = {-30, -30, -30, -30};
int8_t stop[4] = {0, 0, 0, 0};
int32_t enc[4];

int8_t currentTargetSpeed[4];

SemaphoreHandle_t xIIC_Mutex;

void IIC_Mutex_Init(void)
{
    xIIC_Mutex = xSemaphoreCreateMutex();
		motorSpeedQueue = osMessageQueueNew(8, sizeof(MotorSpeedMsg_t), NULL);

}

		
void MotorControl_Task(void *argument)
{
    SoftIIC_Init();	
    Motor_Init(MOTOR_TYPE_JGB37_520_12V_110RPM, 0);
    osDelay(100);

    MotorSpeedMsg_t recvMsg;

    while (1)
    {
        // 等待其他任务发送速度控制指令（最多等100ms）
        if (osMessageQueueGet(motorSpeedQueue, &recvMsg, NULL, osWaitForever) == osOK)
        {
            Motor_SetSpeed(recvMsg.speed);
        }

        // 可选延时或看门狗喂狗
        osDelay(10);
    }
}

void Motor_SetSpeed_Async(int8_t m1, int8_t m2, int8_t m3, int8_t m4)//外部调用调整电机转速的函数，21对应100RPM，最大转速约为300，建议设置不超过60
{
    MotorSpeedMsg_t msg;

    // 修正方向（如果硬件方向反了，可统一取负）
    msg.speed[0] = -m1;
    msg.speed[1] = -m2;
    msg.speed[2] = -m3;
    msg.speed[3] = -m4;

    osMessageQueuePut(motorSpeedQueue, &msg, 0, 0);  // 非阻塞发送
}


void Motor_SetSpeed(int8_t speed[4])
{
    memcpy(currentTargetSpeed, speed, sizeof(currentTargetSpeed));  // 记录当前目标
    Motor_I2C_Write(MOTOR_FIXED_SPEED_ADDR, speed, 4);
		
}

void Motor_Stop(void)
{
    int8_t stop[4] = {0, 0, 0, 0};
    Motor_SetSpeed(stop);
}

void Motor_Init(uint8_t motor_type, uint8_t polarity)
{
    Motor_I2C_Write(MOTOR_TYPE_ADDR, (int8_t *)&motor_type, 1);
    osDelay(5);
    Motor_I2C_Write(MOTOR_ENCODER_POLARITY_ADDR, (int8_t *)&polarity, 1);
    osDelay(5);
}

void Motor_ReadEncoder(int32_t encoder[4])
{
    uint8_t buf[16];
    Motor_I2C_Read(MOTOR_ENCODER_TOTAL_ADDR, buf, 16);
    for (uint8_t i = 0; i < 4; i++)
    {
        encoder[i] = (int32_t)(
            (buf[i*4 + 0]) |
            (buf[i*4 + 1] << 8) |
            (buf[i*4 + 2] << 16) |
            (buf[i*4 + 3] << 24)
        );
    }
}

uint8_t Motor_I2C_Write(uint8_t reg, int8_t *buf, uint8_t len)
{
    SoftIIC_Start();
    SoftIIC_SendByte((MOTOR_I2C_ADDR << 1) | 0);  // 写命令
    if (SoftIIC_WaitAck()) return 1;

    SoftIIC_SendByte(reg);
    if (SoftIIC_WaitAck()) return 1;

    for (uint8_t i = 0; i < len; i++)
    {
        SoftIIC_SendByte((uint8_t)buf[i]);  // 明确地强转一下更稳
        if (SoftIIC_WaitAck()) return 1;
    }

    SoftIIC_Stop();
    return 0;
}


uint8_t Motor_I2C_Read(uint8_t reg, uint8_t *buf, uint8_t len)
{
    SoftIIC_Start();
    SoftIIC_SendByte((MOTOR_I2C_ADDR << 1) | 0);  // 先写寄存器地址
    if (SoftIIC_WaitAck()) return 1;

    SoftIIC_SendByte(reg);
    if (SoftIIC_WaitAck()) return 1;

    SoftIIC_Start();  // 重新开始读
    SoftIIC_SendByte((MOTOR_I2C_ADDR << 1) | 1);  // 读命令
    if (SoftIIC_WaitAck()) return 1;

    for (uint8_t i = 0; i < len; i++)
    {
        buf[i] = SoftIIC_ReadByte(i != (len - 1));  // 最后一个读NAK
    }

    SoftIIC_Stop();
    return 0;
}
