#include "bt_comm.h"
#include "usart.h"
#include "imu901.h"     // ���� imu901_attitude_t �� imu901_get_attitude()
#include "math.h"       // fabs
#include "motor_monitor.h"
#include "motor_iic.h"
#include "servo.h"
#include "bsp_vl53l0x.h"
#include "chassis.h"
#include "motor_monitor.h"
#include <stdbool.h>

// �����������ⲿ����
extern uint16_t hcsr04_distance_mm;
extern int rpm[4];
extern int flag;
float last_deal;
float last_deal_xia;
int32_t up_sum = 0;
int32_t down_sum = 0;
int cnttt=0;
int flag0 = 0;
int stop_flag = 0;
float r ;
float theta;
float phi;
float last_angle = 0.0f;  // ��һ�μ�¼�ĽǶ�

//float speed;
//float wheel_circumference;
//float wheel_diameter_m;
//float avg_rpm;
//float avg_rpmppp;
volatile float angle;
uint8_t rx_byte;
// ����ͨ������


void BT_Comm_Task(void *argument)
{
    const float wheel_circumference = 3.1416f * 65.0f;  // mm
    osDelay(1000);  // �ȴ�IMU�ʹ������ȶ�

    for (;;)
    {		if(flag ==1)
					{
						break;
					}
        // ��ȡ���������루��λ��cm��
        float distance_cm = (float)hcsr04_distance_mm;

        // ����ƽ��RPM
        float avg_rpm = ((float)rpm[0] + (float)rpm[1]) / 2.0f;

        // ��ȡ��̬��
        imu901_attitude_t att = imu901_get_attitude();
        float pitch_angle = -(att.roll - 7.7f);  // ���� pitch

        // ���;�����RPM֡
					if(flag !=1)
					{
        send_float_frame(DISTANCE_ID, distance_cm, DISTANCE_TAIL);
        send_float_frame(SPEED_ID, avg_rpm, SPEED_TAIL);
					}
        // ����Ƕ���Ч��ӽ�ƽ�棬��������
        if (isnan(pitch_angle) || (pitch_angle > -5.0f && pitch_angle < 5.0f))
        {
            pitch_angle = 0.0f;
            osDelay(100);
            continue;
        }

        // ���ݽǶȷ���������»����¾���
        float delta_distance_mm;
        if (pitch_angle > 0.0f)  // ����
        {
            if (last_deal != delta)
            {
                up_sum -= delta;
            }
            last_deal = delta;

            delta_distance_mm = (up_sum / (PULSE_PER_TURN * GEAR_RATIO)) * wheel_circumference;

            send_float_frame(ANGLE_ID, pitch_angle, ANGLE_TAIL);
            send_float_frame(LENGTH_ID, delta_distance_mm, LENGTH_TAIL);
        }
        else  // ����
        {		stop_flag = 1;
            if (last_deal_xia != delta)
            {
                down_sum -= delta;
            }
            last_deal_xia = delta;

            delta_distance_mm = (down_sum / (PULSE_PER_TURN * GEAR_RATIO)) * wheel_circumference;

            send_float_frame(ANGLE_ID_DOWN, pitch_angle, ANGLE_TAIL_DOWN);
            send_float_frame(LENGTH_ID_DOWN, delta_distance_mm, LENGTH_TAIL_DOWN);
        }

        osDelay(100);  // �������� 100ms
				
    }
		
		for (;;)
		{
				// ��ǰ���ֵ
				float r = (float)vl53l0x_distance_mm;

				// ��ǰ����Ƕ�
				float theta = ((float)servo_angle[0] - 500.0f) * 180.0f / 2000.0f;
				float phi   = ((float)servo_angle[1] - 500.0f) * 180.0f / 2000.0f;

				// �жϽǶ��Ƿ�仯 & �����Ƿ���Ч
				bool angle_changed = (phi != last_angle);      // ���߼��ݲ��ж� fabs(theta - last_angle) > 0.01
				bool distance_valid = (r < 350.0f);               // �趨��ֵ����λ mm

				if (angle_changed && distance_valid)
				{
					send_float_frame(0xd5, r + 60.0f, 0x5d);
					osDelay(30);
					send_float_frame(0xd6, theta, 0x6d);
					osDelay(30);
					send_float_frame(0xd7, phi, 0x7d);
					osDelay(30);
					last_angle = phi;  // ���¼�¼�ĽǶ�
				}

				osDelay(10);
		}
}



void send_float_frame(uint8_t header2, float value, uint8_t tail)
{
    uint8_t frame[7];
    uint8_t *data_ptr = (uint8_t *)&value;  // ��ȡ float ���ڴ��ַ

    frame[0] = FRAME_HEAD;
		frame[1] = header2;
    frame[2] = data_ptr[3];  // ��λ����������λ��
    frame[3] = data_ptr[2];
    frame[4] = data_ptr[1];
    frame[5] = data_ptr[0];  // ��λ
    frame[6] = tail;
		
		
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}


void send_r_theta_phi(float r, float theta, float phi)
{
    uint8_t frame[15];
    uint8_t *ptr;

    // ֡ͷ
    frame[0] = 0xD5;
    frame[1] = 0xD6;

    // r
    ptr = (uint8_t *)&r;
    frame[2] = ptr[3];
    frame[3] = ptr[2];
    frame[4] = ptr[1];
    frame[5] = ptr[0];

    // theta
    ptr = (uint8_t *)&theta;
    frame[6] = ptr[3];
    frame[7] = ptr[2];
    frame[8] = ptr[1];
    frame[9] = ptr[0];

    // phi
    ptr = (uint8_t *)&phi;
    frame[10] = ptr[3];
    frame[11] = ptr[2];
    frame[12] = ptr[1];
    frame[13] = ptr[0];

    // ֡β
    frame[14] = 0x6D;

    // ����
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}

void WaitForBT_ReadySignal(void)
{
    rx_byte = 0;

    while (1)
    {
        // �������� 1 �ֽڣ���ʱʱ�� HAL_MAX_DELAY ��ʾ���޵�
        if (HAL_UART_Receive(&huart1, &rx_byte, 1, HAL_MAX_DELAY) == HAL_OK)
        {
            if (rx_byte == 0xFF)
            {
                // �յ� 0xFF�������ȴ���������һ��
                break;
            }
        }

        osDelay(10);  // С��ʱ��ֹ����
    }
}

void send_float_frame_PTZ(uint8_t header2, float value, uint8_t tail)
{
    uint8_t frame[7];
    uint8_t *data_ptr = (uint8_t *)&value;  // ��ȡ float ���ڴ��ַ

    frame[0] = MAP_HEAD;
		frame[1] = header2;
    frame[2] = data_ptr[3];  // ��λ����������λ��
    frame[3] = data_ptr[2];
    frame[4] = data_ptr[1];
    frame[5] = data_ptr[0];  // ��λ
    frame[6] = tail;
		
		
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}