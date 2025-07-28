#include "motorMonitor.h"
#include "mid_timer.h"
#include "hw_encoder.h"

#define ENC_PPR        11           // 编码器每圈脉冲数
#define GEAR_RATIO     30.0f        // 电机减速比
#define UPDATE_INTERVAL_SEC 0.02f   // 每 20ms 调用一次 encoder_update()

float get_output_rpm_l(void)
{
    int pulses = get_encoder_count_l();  // 20ms内的跳变脉冲数
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