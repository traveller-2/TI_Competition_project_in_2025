#ifndef _MOTOR_MONITOR_H_
#define _MOTOR_MONITOR_H_

#include "mid_timer.h"
#include "hw_encoder.h"

// ������ÿȦ��������PPR��
#define ENC_PPR        11

// ������ٱ�
#define GEAR_RATIO     30.0f

// ���������¼�����룩������Ӧ��ʱ������ encoder_update() ������
#define UPDATE_INTERVAL_SEC 0.02f

#ifdef __cplusplus
extern "C" {
#endif

// ��ȡ���������ת�٣�RPM��
float get_output_rpm_l(void);

// ��ȡ���������ת�٣�RPM��
float get_output_rpm_r(void);

#ifdef __cplusplus
}
#endif

#endif  // _MOTOR_MONITOR_H_
