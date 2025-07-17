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
//static void send_motor_velocity(uint8_t id, uint16_t rpm, uint8_t acc, uint8_t dir);   // �����ٶȿ���֡
//static void send_motor_origin(uint8_t id, uint16_t rpm, bool sync);                    // ���͹������֡
//static void send_motor_frame(const uint8_t *data, uint8_t len);                        // ͨ��֡���ͺ���

/**
 * @brief ��̨�����������
 *        �ȴ������еĿ��������װЭ��֡��ͨ�����ڷ���
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

        osDelay(1);  // Ϊ���������ó�ʱ��Ƭ
    }
}



void MyControlTask(void *argument) {
    GimbalControlCmd_t cmd;
		// ����Ϊ��ת�����ϣ����Ϊ����㡱��λ��
		// Ȼ��ִ��������������ô�λ��Ϊ��㣨����������棩
		osDelay(1500);
		send_motor_set_zero_position(1, 1);  // �� ID=1 �ĵ�������õ�ǰλ��Ϊ��㲢����

    osDelay(500);

    //�ٶ�ģʽ���ƣ���ת200 RPM��10���ٶȵ�
		memset(&cmd, 0, sizeof(cmd));  // ��������ֶ�
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_SPEED;
    cmd.rpm = 200;
    cmd.acc = 10;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(5000);
	
	  //�ٶ�ģʽ���ƣ���ת 0 RPM��10���ٶȵ�
		memset(&cmd, 0, sizeof(cmd));  // ��������ֶ�
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_SPEED;
    cmd.rpm = 0;
    cmd.acc = 10;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(5000);


    //λ��ģʽ���ƣ���ת10Ȧ��32000���壩��200RPM���޼��ٶ����ߣ����ģʽ
		memset(&cmd, 0, sizeof(cmd));  // ��������ֶ�
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_POSITION;
    cmd.rpm = 200;
    cmd.acc = 10;
    cmd.pulse = 32000;
    cmd.pos_mode = 0;  // 0 ���λ��ģʽ��1 ����λ��ģʽ
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);
		
		osDelay(10000);
		
		 //����ģʽ���ƣ���Ȧ�ͽ�����
		memset(&cmd, 0, sizeof(cmd));  // ��������ֶ�
    cmd.id = 0x01;
    cmd.mode = MOTOR_CTRL_ZERO;
    cmd.zero_mode = 0;
    osMessageQueuePut(gimbalCmdQueue, &cmd, 0, 0);

    osDelay(1000);

    while (1) {
        osDelay(1000); // ��ֹ����
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
void send_motor_velocity(uint8_t id, int16_t rpm, uint8_t acc, uint8_t dir) {
    uint8_t buf[8];
    buf[0] = id;
    buf[1] = 0xF6;
    buf[2] = dir;
    buf[3] = (uint8_t)((rpm >> 8) & 0xFF);
    buf[4] = (uint8_t)(rpm & 0xFF);
    buf[5] = acc;
    buf[6] = 0x00; // �����ö��ͬ��
    buf[7] = 0x6B; // У���ֽڣ������㷨��д����
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
    buf[11] = 0x00; // �����ö��ͬ��
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
    buf[0] = id;          // ��� ID
    buf[1] = 0x93;        // ָ�����ͣ��������
    buf[2] = 0x88;        // �̶���������
    buf[3] = save_flag;   // �Ƿ�洢��0x01 Ϊ�洢������
    buf[4] = 0x6B;        // �̶�У���ֽ�

    send_motor_frame(buf, sizeof(buf));
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
