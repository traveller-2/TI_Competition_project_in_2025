#include "imu901.h"
#include "usart.h"      // ���� huart
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"


#define HUART         huart3//�滻����
#define USART 				USART3
#define UP_BYTE1      0x55
#define UP_BYTE2      0x55
#define UP_ATTITUDE   0x01

volatile uint8_t imu_uart_active = 0;

static enum {
    WAIT_HEAD1,
    WAIT_HEAD2,
    WAIT_MSGID,
    WAIT_LEN,
    WAIT_DATA,
    WAIT_CHECK
} state = WAIT_HEAD1;

static struct {
    uint8_t startByte1;
    uint8_t startByte2;
    uint8_t msgID;
    uint8_t dataLen;
    uint8_t data[32];
    uint8_t checkSum;
} packet;

static uint8_t index = 0;
static uint8_t cksum = 0;
static imu901_attitude_t attitude;

uint8_t imu_rx_byte = 0;  // IMU���ڽ��յĵ��ֽڻ���


void imu901_light_init(void) {
    state = WAIT_HEAD1;
    index = 0;
    cksum = 0;
}

void imu901_uart_start(void)
{
    HAL_UART_Receive_IT(&HUART, &imu_rx_byte, 1);
}

uint8_t imu901_light_receive(uint8_t ch) {
    switch (state) {
        case WAIT_HEAD1:
            if (ch == UP_BYTE1) {
                state = WAIT_HEAD2;
                packet.startByte1 = ch;
                cksum = ch;
            }
            break;

        case WAIT_HEAD2:
            if (ch == UP_BYTE2) {
                state = WAIT_MSGID;
                packet.startByte2 = ch;
                cksum += ch;
            } else {
                state = WAIT_HEAD1;
            }
            break;

        case WAIT_MSGID:
            packet.msgID = ch;
            cksum += ch;
            state = WAIT_LEN;
            break;

        case WAIT_LEN:
            if (ch <= sizeof(packet.data)) {
                packet.dataLen = ch;
                cksum += ch;
                index = 0;
                state = ch > 0 ? WAIT_DATA : WAIT_CHECK;
            } else {
                state = WAIT_HEAD1;
            }
            break;

        case WAIT_DATA:
            packet.data[index++] = ch;
            cksum += ch;
            if (index == packet.dataLen) {
                state = WAIT_CHECK;
            }
            break;

        case WAIT_CHECK:
            if (cksum == ch && packet.msgID == UP_ATTITUDE) {
                int16_t roll  = (int16_t)(packet.data[1] << 8 | packet.data[0]);
                int16_t pitch = (int16_t)(packet.data[3] << 8 | packet.data[2]);
                int16_t yaw   = (int16_t)(packet.data[5] << 8 | packet.data[4]);
                attitude.roll  = roll  * 180.0f / 32768;
                attitude.pitch = pitch * 180.0f / 32768;
                attitude.yaw   = yaw   * 180.0f / 32768;
            }
            state = WAIT_HEAD1;
            break;

        default:
            state = WAIT_HEAD1;
            break;
    }
    return 0;
}

imu901_attitude_t imu901_get_attitude(void) {
    imu901_attitude_t temp;

    if (imu_uart_active == 0) {
        //  û���������жϣ����� 555 ��Ϊ��Ч����
        temp.pitch = 555;
        temp.roll  = 555;
        temp.yaw   = 555;
    } else {
        //  �����жϣ����Է��ؽ���������
				taskENTER_CRITICAL();      // ��ʼ�ٽ���
        temp = attitude;
        taskEXIT_CRITICAL();       // �����ٽ���
    }

    return temp;
}


// �жϻص���ÿ����1�ֽھʹ���
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART)
    {

        imu_uart_active = 1;  // ? ��ʾ�жϴ�����

        imu901_light_receive(imu_rx_byte);  // ����Э�����
        HAL_UART_Receive_IT(&HUART, &imu_rx_byte, 1);  // ��������һ���ֽ�
    }
}

