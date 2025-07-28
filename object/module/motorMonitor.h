#ifndef _MOTOR_MONITOR_H_
#define _MOTOR_MONITOR_H_

#include "mid_timer.h"
#include "hw_encoder.h"

// 编码器每圈脉冲数（PPR）
#define ENC_PPR        11

// 电机减速比
#define GEAR_RATIO     30.0f

// 编码器更新间隔（秒）——对应定时器调用 encoder_update() 的周期
#define UPDATE_INTERVAL_SEC 0.02f

#ifdef __cplusplus
extern "C" {
#endif

// 获取左轮输出轴转速（RPM）
float get_output_rpm_l(void);

// 获取右轮输出轴转速（RPM）
float get_output_rpm_r(void);

#ifdef __cplusplus
}
#endif

#endif  // _MOTOR_MONITOR_H_
