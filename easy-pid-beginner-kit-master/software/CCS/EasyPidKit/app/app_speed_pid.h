#ifndef	__APP_SPEED_PID_H__
#define __APP_SPEED_PID_H__

#include "mid_pid.h"

//extern int target_speed;
extern int PWM;



void speed_pid_init(void);
PID* get_speed_pid(void);
int get_speed_pid_target(void);
PID motor_velcity_control(int target);
void set_speed_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag);
#endif
