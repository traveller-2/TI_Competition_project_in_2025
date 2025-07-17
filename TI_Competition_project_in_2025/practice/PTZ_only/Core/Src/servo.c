#include "servo.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

#define SERVO_UART                &huart2  // ʹ�� USART2
#define SERVO_TX_BUF_LEN         64
#define SERVO_QUEUE_LEN          10

#define MAX_SERVO_NUM  2  // ���֧��2�����

volatile uint16_t servo_angle[MAX_SERVO_NUM] = {0};  // �洢�Ƕ�ֵ
volatile uint8_t  servo_status[MAX_SERVO_NUM] = {0}; // 0���� 1��ȡʧ��

osMutexId_t servoUartMutex;

/***********************************˵��*********************************************
	*	uint16_t angle = servo_angle[1];  // ��ȡ1�Ŷ���Ƕ�
	*	if (servo_status[1] == 0)
	*	{
  * 	 // ������ȡ
	*	}
	*	else
	*	{
  *	 // ��ȡʧ�ܣ����ܴ���δ���ӡ����û��Ӧ��
	*	}
	************************************************************************************/


//void ServoReadTask(void *argument)
//{
//    while (1)
//    {
//        for (uint8_t i = 0; i < MAX_SERVO_NUM; i++)
//        {
//            Servo_ReadAngleOnce(i);
//            osDelay(50);  // ������֮�������ӳ�
//        }

//        osDelay(100); // ÿ 100ms ����һ�����нǶ�
//    }
//}

void Servo_ReadAngleOnce(uint8_t id)
{
    char tx_buf[20];
    char rx_buf[20];

    snprintf(tx_buf, sizeof(tx_buf), "#%03dPRAD!", id);
    
    osMutexAcquire(servoUartMutex, osWaitForever);  // ����

    HAL_UART_Transmit(SERVO_UART, (uint8_t *)tx_buf, strlen(tx_buf), HAL_MAX_DELAY);

    // ��һ�ν��գ����ԣ�������
    HAL_UART_Receive(SERVO_UART, (uint8_t *)rx_buf, sizeof(rx_buf), 100);

    // �ڶ��ν��գ������ش�����
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

/***********************************˵��*********************************************
	*Servo_Init();                    // ��һ������ʼ��
	*Servo_SetID(0, 1);               // �ڶ��������Σ����޸�ID��ÿ�����Ψһ��
	*Servo_SetMode(1, 1);             // �����������ö��ģʽ����270�ȣ�
	*
	* ���ƶ�����
	*Servo_QueueControl(1, 1500, 1000);
	*Servo_QueueControl(2, 2000, 1000);
	*
	* ��ѡ����ȡ����Ƕ�
	*char buffer[20];
	*Servo_ReadAngle(1, buffer, 100);
	************************************************************************************/

// ��ʼ���������������� + ��������
void Servo_Init(void)
{
    servoQueueHandle = osMessageQueueNew(SERVO_QUEUE_LEN, sizeof(ServoCmd_t), NULL);
		servoUartMutex = osMutexNew(NULL);
}

// ����������񣨴��ڷ����̣߳�
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
						osMutexAcquire(servoUartMutex, osWaitForever);  //  ����
						HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
						osMutexRelease(servoUartMutex);  //  ����
        }
    }
}

// �ⲿ���ýӿڣ����Ͷ����������
void Servo_QueueControl(uint8_t id, uint16_t pwm, uint16_t time_ms)
{
    ServoCmd_t cmd = {id, pwm, time_ms};
    osMessageQueuePut(servoQueueHandle, &cmd, 0, 0);

    // ͬ�����½Ƕ�ֵ��ģ��ʵ��λ�ã�
    servo_angle[id] = pwm;
    servo_status[id] = 0;
}

// ���ö�� ID
void Servo_SetID(uint8_t old_id, uint8_t new_id)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPID%03d!", old_id, new_id);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}

// ���ö������ģʽ��1~8��
void Servo_SetMode(uint8_t id, uint8_t mode)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPMOD%d!", id, mode);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}

// ��ȡ�Ƕȣ��� TX-RX �̽ӣ�
void Servo_ReadAngle(uint8_t id, char *ret_buf, uint16_t timeout)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "#%03dPRAD!", id);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
    HAL_UART_Receive(SERVO_UART, (uint8_t *)ret_buf, 16, timeout);
}

// ����ͬ������ָ����ֶ�ƴ�Ӻ��룩
void Servo_MultiControl(const char *commands)
{
    snprintf(servo_tx_buf, SERVO_TX_BUF_LEN,
             "{G0000%s}", commands);
    HAL_UART_Transmit(SERVO_UART, (uint8_t *)servo_tx_buf, strlen(servo_tx_buf), HAL_MAX_DELAY);
}



/************************************************************************************************************
	*																					������PWM���																										*
	***********************************************************************************************************/
static uint16_t current_pulse = 1500; // Ĭ���м�λ��

void servo_init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
}

void servo_set_angle(uint8_t angle)
{
    if (angle > 180) angle = 180;
    current_pulse = 500 + ((uint32_t)angle * 2000) / 180; // ӳ��Ƕȵ�����
}

// ��� FreeRTOS ����
//void ServoTask(void *argument)
//{
//    while (1)
//    {
//        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, current_pulse);  // ���� PWM
//        osDelay(20); // ���ˢ������Ϊ 20ms
//    }
//}
