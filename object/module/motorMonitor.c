#include "motorMonitor.h"
#include "mid_timer.h"
#include "hw_encoder.h"

#define ENC_PPR        11           // ������ÿȦ������
#define GEAR_RATIO     30.0f        // ������ٱ�
#define UPDATE_INTERVAL_SEC 0.02f   // ÿ 20ms ����һ�� encoder_update()

float get_output_rpm_l(void)
{
    int pulses = get_encoder_count_l();  // 20ms�ڵ�����������
    float motor_rpm = (pulses / (float)ENC_PPR) * (60.0f / UPDATE_INTERVAL_SEC);
    float output_rpm = motor_rpm / GEAR_RATIO;
    return output_rpm;
}

float get_output_rpm_r(void)
{
    int pulses = get_encoder_count_r();
    float motor_rpm = (pulses / (float)ENC_PPR) * (60.0f / UPDATE_INTERVAL_SEC);
    float output_rpm = motor_rpm / GEAR_RATIO;
    return output_rpm;
}