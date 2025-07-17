#include "bt_comm.h"
#include "usart.h"
#include "imu901.h"     // 包含 imu901_attitude_t 和 imu901_get_attitude()
#include "math.h"       // fabs
#include "motor_monitor.h"
#include "motor_iic.h"
#include "servo.h"
#include "bsp_vl53l0x.h"
#include "chassis.h"
#include "motor_monitor.h"
#include <stdbool.h>

// 从主程序导入外部数据
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
float last_angle = 0.0f;  // 上一次记录的角度

//float speed;
//float wheel_circumference;
//float wheel_diameter_m;
//float avg_rpm;
//float avg_rpmppp;
volatile float angle;
uint8_t rx_byte;
// 蓝牙通信任务


void BT_Comm_Task(void *argument)
{
    const float wheel_circumference = 3.1416f * 65.0f;  // mm
    osDelay(1000);  // 等待IMU和传感器稳定

    for (;;)
    {		if(flag ==1)
					{
						break;
					}
        // 获取超声波距离（单位：cm）
        float distance_cm = (float)hcsr04_distance_mm;

        // 计算平均RPM
        float avg_rpm = ((float)rpm[0] + (float)rpm[1]) / 2.0f;

        // 获取姿态角
        imu901_attitude_t att = imu901_get_attitude();
        float pitch_angle = -(att.roll - 7.7f);  // 修正 pitch

        // 发送距离与RPM帧
					if(flag !=1)
					{
        send_float_frame(DISTANCE_ID, distance_cm, DISTANCE_TAIL);
        send_float_frame(SPEED_ID, avg_rpm, SPEED_TAIL);
					}
        // 如果角度无效或接近平面，跳过处理
        if (isnan(pitch_angle) || (pitch_angle > -5.0f && pitch_angle < 5.0f))
        {
            pitch_angle = 0.0f;
            osDelay(100);
            continue;
        }

        // 根据角度方向计算上坡或下坡距离
        float delta_distance_mm;
        if (pitch_angle > 0.0f)  // 上坡
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
        else  // 下坡
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

        osDelay(100);  // 任务周期 100ms
				
    }
		
		for (;;)
		{
				// 当前测距值
				float r = (float)vl53l0x_distance_mm;

				// 当前舵机角度
				float theta = ((float)servo_angle[0] - 500.0f) * 180.0f / 2000.0f;
				float phi   = ((float)servo_angle[1] - 500.0f) * 180.0f / 2000.0f;

				// 判断角度是否变化 & 距离是否有效
				bool angle_changed = (phi != last_angle);      // 或者加容差判断 fabs(theta - last_angle) > 0.01
				bool distance_valid = (r < 350.0f);               // 设定阈值，单位 mm

				if (angle_changed && distance_valid)
				{
					send_float_frame(0xd5, r + 60.0f, 0x5d);
					osDelay(30);
					send_float_frame(0xd6, theta, 0x6d);
					osDelay(30);
					send_float_frame(0xd7, phi, 0x7d);
					osDelay(30);
					last_angle = phi;  // 更新记录的角度
				}

				osDelay(10);
		}
}



void send_float_frame(uint8_t header2, float value, uint8_t tail)
{
    uint8_t frame[7];
    uint8_t *data_ptr = (uint8_t *)&value;  // 获取 float 的内存地址

    frame[0] = FRAME_HEAD;
		frame[1] = header2;
    frame[2] = data_ptr[3];  // 高位（包含符号位）
    frame[3] = data_ptr[2];
    frame[4] = data_ptr[1];
    frame[5] = data_ptr[0];  // 低位
    frame[6] = tail;
		
		
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}


void send_r_theta_phi(float r, float theta, float phi)
{
    uint8_t frame[15];
    uint8_t *ptr;

    // 帧头
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

    // 帧尾
    frame[14] = 0x6D;

    // 发送
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}

void WaitForBT_ReadySignal(void)
{
    rx_byte = 0;

    while (1)
    {
        // 阻塞接收 1 字节，超时时间 HAL_MAX_DELAY 表示无限等
        if (HAL_UART_Receive(&huart1, &rx_byte, 1, HAL_MAX_DELAY) == HAL_OK)
        {
            if (rx_byte == 0xFF)
            {
                // 收到 0xFF，跳出等待，继续下一步
                break;
            }
        }

        osDelay(10);  // 小延时防止过载
    }
}

void send_float_frame_PTZ(uint8_t header2, float value, uint8_t tail)
{
    uint8_t frame[7];
    uint8_t *data_ptr = (uint8_t *)&value;  // 获取 float 的内存地址

    frame[0] = MAP_HEAD;
		frame[1] = header2;
    frame[2] = data_ptr[3];  // 高位（包含符号位）
    frame[3] = data_ptr[2];
    frame[4] = data_ptr[1];
    frame[5] = data_ptr[0];  // 低位
    frame[6] = tail;
		
		
    HAL_UART_Transmit(&huart1, frame, sizeof(frame), HAL_MAX_DELAY);
}