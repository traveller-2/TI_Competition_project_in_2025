#ifndef	__APP_DISTANCE_PID_H__
#define __APP_DISTANCE_PID_H__

#include "mid_pid.h"

//目标脉冲数 = ( 目标距离 / 轮子周长 ) × 编码器每转脉冲数 × 减速比
//示例：轮子直径10cm → 周长=31.4cm，编码器1000脉冲/转，减速比10:1 → 1米对应脉冲数 = ( 100 / 31.4 ) × 1000 × 10 ≈ 31847
//示例：轮子直径65mm → 周长=204.20mm，编码器2496脉冲/转，减速比48:1 → 1米对应脉冲数 = ( 1,000 / 204.20 ) × 2496 × 48 ≈ 31847

#define MOTOR_REDUCTION_RATIO   48    //电机减速比 1：48
#define ENCODER_RESOLUTION  	1248      //编码器分辨率 1248脉冲/转
#define TIRE_CIRCUMFERENCE 		204.20f    //轮胎周长单位为毫米 TIRE_DIAMETER x 3.1415926
#define TIRE_DIAMETER  			65         //轮胎直径 单位毫米

#define DEGREES_PER_PULSE  		0.28846f	// 每脉冲度数 360 / ENCODER_RESOLUTION
#define DEFAULT_ANGLE			90			// 起始默认旋转角度

void distance_pid_init(void);
PID motor_distance_control(int target_angle);
PID* get_distance_pid(void);
int get_distance_pid_target(void);
void set_distance_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag);
#endif
