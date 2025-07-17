#include "cmsis_os.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "motor_iic.h"
#include "OLED.h"
#include "motor_monitor.h"
#include "math.h"       // fabs

#define PULSES_PER_REVOLUTION (PULSE_PER_TURN * GEAR_RATIO)
#define PULSE_PER_TURN       44
#define GEAR_RATIO           30
#define MONITOR_INTERVAL_MS  200
#define dt                   (MONITOR_INTERVAL_MS / 1000.0f)

int rpm[4] = {0};  // ȫ�ֱ������� OLED ʹ��
float avg_rpm;
float wheel_diameter_m;
float wheel_circumference;
volatile float speed;
int32_t delta;


void MotorRPM_Update_Task(void *argument)
{
    while (1)
    {
        Motor_UpdateRPM();
			avg_rpm = (fabs((float)rpm[0]) + fabs((float)rpm[1]))/ 2.0f;

			//  �������٣���λ m/s��
			wheel_diameter_m = 0.065f;  // 65mm
			wheel_circumference = 3.1416f * wheel_diameter_m;
			speed = (avg_rpm / 60.0f) * wheel_circumference*1000;
       osDelay(MONITOR_INTERVAL_MS);  // ����һ��
    }
}

void Motor_UpdateRPM(void)
{
    static int32_t last_encoder[4] = {0};
    int32_t encoder_now[4];
    Motor_ReadEncoder(encoder_now);

    for (int i = 0; i < 2; i++)
    {
        delta = encoder_now[i] - last_encoder[i];
        last_encoder[i] = encoder_now[i];

        // ���Ƽ����쳣ֵ����ֹ��������
        if (delta > 10000 || delta < -10000)
            delta = 0;

        float pps = delta / (dt); // ����/��
        float rps = pps / (PULSE_PER_TURN * GEAR_RATIO); // ת/��
        float rpm_f = rps * 60.0f;
        rpm[i] = -(int)(rpm_f + 0.5f);  // ��������Ϊ����
    }
}
