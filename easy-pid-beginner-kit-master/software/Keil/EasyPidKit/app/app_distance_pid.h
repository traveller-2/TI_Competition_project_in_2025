#ifndef	__APP_DISTANCE_PID_H__
#define __APP_DISTANCE_PID_H__

#include "mid_pid.h"

#define MOTOR_REDUCTION_RATIO   48    		// ������ٱ� 1��48
#define ENCODER_RESOLUTION  	1248      	// ���һȦ�ı����������� 1248����/ת
#define DEGREES_PER_PULSE  		0.28846f	// ÿ������� 360 / ENCODER_RESOLUTION
#define DEFAULT_ANGLE			90			// ��ʼĬ����ת�Ƕ�

void distance_pid_init(void);
PID* get_distance_pid(void);
int get_distance_pid_target(void);
PID motor_distance_control(int target_angle);
void set_distance_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag);
#endif
