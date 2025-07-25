#include "app_speed_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "app_ui.h"
#include "hw_uart.h"
#include "stdio.h"
#include "ti/driverlib/m0p/dl_core.h"

PID speed_pid;


// 定义滤波器系数，这个值决定了滤波器的截止频率和滤波效果
#define ALPHA 0.1
float filtered_speed = 0.0; // 滤波后的速度值，初始设为0

// 低通滤波器函数
float lowpass_filter(float new_value, float last_value) {
    return ALPHA * new_value + (1.0 - ALPHA) * last_value;
}

void speed_pid_init(void)
{
	//35 6 10
	//200, 10, 10,
    pid_init(&speed_pid, 35, 6, 10, 9999, 9999,50);
}

PID* get_speed_pid(void)
{
	return &speed_pid;
}
int get_speed_pid_target(void)
{
	return speed_pid.target;
}

PID motor_velcity_control(int target)
{
	//char buff[50]={0};
	int PWM;
	filtered_speed = lowpass_filter(get_encoder(), filtered_speed);

	PWM = pid_calc(&speed_pid, target, filtered_speed);//指定两轮速度
	
	//控速
	// sprintf(buff,"get_encoder:%d,%d,%d\n", target,(int)filtered_speed,get_encoder());
	// uart0_send_string(buff);
	delay_cycles(80000*5);

	if( PWM > 0 )
	{
		set_motor(0, PWM);
	}
	else if( PWM < 0 )
	{
		PWM = -PWM;
		set_motor(PWM, 0);
	}
	return speed_pid;
}

void set_speed_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag)
{
	if( add_or_subtract_flag == 0 ) //加
	{
		switch(select)
		{
			case 0://调整P
				pid_value->kp = pid_value->kp + 0.1;
				break;
			case 1://调整I
				pid_value->ki = pid_value->ki + 0.1;
				break;
			case 2://调整D
				pid_value->kd = pid_value->kd + 0.1;
				break;			
			case 3://调整target
				if( pid_value->target < 100 )
					pid_value->target = pid_value->target + 1;
				break;		
		}
	} 
	else //减
	{
		switch(select)
		{
			case 0://调整P
				if(  pid_value->kp > 0 )
					pid_value->kp = pid_value->kp - 0.1;
				pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp;  // 消除负零
				break;
			case 1://调整I
				if( pid_value->ki > 0 )
					pid_value->ki = pid_value->ki - 0.1;
				pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki;  // 消除负零
				break;
			case 2://调整D
				if( pid_value->kd > 0 )
					pid_value->kd = pid_value->kd - 0.1;
				pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd;  // 消除负零
				break;			
			case 3://调整target
				if( pid_value->target > -100 )
					pid_value->target = (pid_value->target - 1);
				break;		
		}	
	}
}