#include "gimbal_motor.h"    // ��̨�������ģ��ͷ�ļ�
#include "bsp_usart.h"       // ���ڵײ㷢�ͷ�װ
#include "cmsis_os.h"        // FreeRTOS�ӿ�
#include <string.h>
#include <stdbool.h>

// === ���ڶ��壨�ɸ�Ϊ�����ã� ===
extern UART_HandleTypeDef huart1;           // ���ھ�����ⲿ���壩
#define GIMBAL_UART  (&huart1)              // ��̨�������ʹ�õĴ��ڣ����� USART1��

// === ͨ��Э�鶨�� ===
#define MOTOR_CHECK_CODE         0x6B        // ֡βУ���룬�̶�ֵ��������֤֡������

// === �����붨�� ===
#define MOTOR_FUNC_VELOCITY_MODE  0xF6       // ����ٶȿ��ƹ�����
#define MOTOR_FUNC_ORIGIN_MODE    0xE3       // �����ԭ����ƹ�����

#define MOTOR_SYNC_DISABLE        0x00       // ��ͬ������
#define MOTOR_SYNC_ENABLE         0x01       // ����ͬ�����ƣ����ڶ�������ȳ�����

// === ��Ϣ���ж����� ===
static osMessageQueueId_t gimbalCmdQueue = NULL;  // ��̨����������о������̬˽�У�

// === �ڲ�����������˽�к����� ===
static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir);   // �����ٶȿ���֡
static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync);                    // ���͹������֡
static void send_motor_frame(const uint8_t *data, uint8_t len);                        // ͨ��֡���ͺ���

/**
 * @brief ��̨�����������
 *        �ȴ������еĿ��������װЭ��֡��ͨ�����ڷ���
 */
void GimbalMotor_Task(void *argument) {
    GimbalControlCmd_t cmd;  // ���ڴ�Ŵ���Ϣ����ȡ���ĵ����������

    while (1) {
        // �����Ϣ�����Ƿ񴴽��ɹ���ͬʱ�����ȴ��µĿ�������������
        if (gimbalCmdQueue != NULL &&
            osMessageQueueGet(gimbalCmdQueue, &cmd, NULL, osWaitForever) == osOK) {

            /**
             * ���������е� rpm �жϵ��ת������
             * - ��� rpm >= 0������Ϊ�ǡ���ת����dir = 0
             * - ��� rpm < 0������Ϊ�ǡ���ת����dir = 1
             * ͬʱȡ rpm �ľ���ֵ��Ϊת��
             */
            uint8_t dir = (cmd.rpm >= 0) ? 0 : 1;           // �����־��0����ת��1����ת��
            uint16_t rpm = (cmd.rpm >= 0) ? cmd.rpm : -cmd.rpm;  // ת��ȡ����ֵ

            // �����ڲ�������װ�����͡��ٶȿ���Э��֡��
            // ʵ��ͨ�����������·�����ָ��
            send_motor_velocity(cmd.id, rpm, cmd.acc, dir);
        }
			osDelay(1);
        // ���û���յ���Ч������Զ��ٴ������ȴ���������ʱ
    }
}

void MyControlTask(void *argument) {
    GimbalControlCmd_t cmd;
		osDelay(1000);  			//�ȴ�����ϵ�
    
		// �� 1 �ŵ������ת�� 300 rpm�����ٶ�Ϊ 15
    cmd.id = 1;
    cmd.rpm = 40;
    cmd.acc = 5;
    GimbalMotor_SendCmd(&cmd);  //  ���Ϳ�������

    osDelay(1000);  // ��һ��

    // �� 2 �ŵ������ת�� 200 rpm�����ٶ�Ϊ 10
    cmd.id = 1;
    cmd.rpm = -20;  // ��ֵ����ת
    cmd.acc = 5;
    GimbalMotor_SendCmd(&cmd);  // ? �ٴη���
		for(;;)
		{
			osDelay(1);
		}
}


/**
 * @brief ��ʼ����̨�����������
 *        Ӧ��ϵͳ��ʼ���׶ε���
 */
void GimbalMotor_QueueInit(void) {
    gimbalCmdQueue = osMessageQueueNew(
        GIMBAL_CMD_QUEUE_LEN,             // ���г��ȣ��ɶ���Ϊ�꣩
        sizeof(GimbalControlCmd_t),       // ÿ��Ԫ�ش�С
        NULL                              // ����ΪĬ��
    );
}

/**
 * @brief ����̨�������������һ����������
 * @param cmd ָ������ṹ���ָ��
 * @return true �ɹ�������У�false ʧ��
 */
bool GimbalMotor_SendCmd(GimbalControlCmd_t *cmd) {
    if (gimbalCmdQueue == NULL || cmd == NULL)
        return false;

    return osMessageQueuePut(gimbalCmdQueue, cmd, 0, 0) == osOK;
}

/**
 * @brief ��װ�ٶȿ���Э��֡������
 * @param id  ���ID
 * @param rpm ���ת�٣�����ֵ��
 * @param acc ���ٶ�
 * @param dir ת������0��ת��1��ת��
 */
static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir) {
    uint8_t frame[8] = {
        id,                             // ���ID
        MOTOR_FUNC_VELOCITY_MODE,      // �����룺�ٶ�ģʽ
        dir,                            // ����
        (uint8_t)(rpm >> 8),            // ��8λ
        (uint8_t)(rpm & 0xFF),          // ��8λ
        acc,                            // ���ٶ�
        MOTOR_SYNC_DISABLE,             // ��ͬ��ģʽ
        MOTOR_CHECK_CODE                // �̶�У����
    };

    send_motor_frame(frame, sizeof(frame));
}

/**
 * @brief ��װ�������Э��֡������
 * @param id   ���ID
 * @param rpm  �����ٶ�
 * @param sync �Ƿ����ö��ͬ��
 */
static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync) {
    uint8_t frame[7] = {
        id,                             // ���ID
        MOTOR_FUNC_ORIGIN_MODE,        // �����룺����
        0x00,                           // ����λ������չ��
        (uint8_t)(rpm >> 8),            // ��8λ�ٶ�
        (uint8_t)(rpm & 0xFF),          // ��8λ�ٶ�
        sync ? MOTOR_SYNC_ENABLE : MOTOR_SYNC_DISABLE,  // ͬ����־
        MOTOR_CHECK_CODE                // У����
    };

    send_motor_frame(frame, sizeof(frame));
}

/**
 * @brief �����������ֽڷ��ͺ���
 * @param data ����ָ֡��
 * @param len  ����֡����
 */
static void send_motor_frame(const uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; ++i) {
        BSP_USART_SendByte(GIMBAL_UART, data[i]);  // ʹ�� BSP ���װ�ķ��ͽӿ�
    }
}
