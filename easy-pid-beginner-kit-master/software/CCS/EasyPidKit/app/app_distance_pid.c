#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "stdlib.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "stdio.h"
#include "hw_uart.h"

PID distance_pid;



void distance_pid_init(void)
{
    pid_init(&distance_pid, 75, 2, 10, 9999, 9999, DEFAULT_ANGLE );
}

PID* get_distance_pid(void)
{
	return &distance_pid;
}
int get_distance_pid_target(void)
{
	return distance_pid.target;
}


PID motor_distance_control(int target_angle)
{
    int PWM=0;
	int target_pulses = 0;
	char buff[50]={0};

	//将角度转换为脉冲
	target_pulses = target_angle / DEGREES_PER_PULSE;

    PWM = pid_calc(&distance_pid, target_pulses, get_temp_encoder());
    
    if( PWM > 0 )
    {
        set_motor(0, PWM);
    }
    else if( PWM < 0 )
    {
        PWM = -PWM;
        set_motor(PWM, 0);
    }
    
	//控速
    // sprintf(buff,"get_encoder:%lld,%d\r\n",get_temp_encoder(),target_pulses);
    // uart0_send_string(buff);
    delay_cycles(80000*5);
	
	return distance_pid;   
}

void set_distance_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag)
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
				if( pid_value->target < 360 )
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
					pid_value->kp = pid_value->kp - 0.1f;
				pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp;  // 消除负零
				break;
			case 1://调整I
				if( pid_value->ki > 0 )
					pid_value->ki = pid_value->ki - 0.1f;
				pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki;  // 消除负零
				break;
			case 2://调整D
				if( pid_value->kd > 0 )
					pid_value->kd = pid_value->kd - 0.1f;
				pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd;  // 消除负零
				break;			
			case 3://调整target
				if( pid_value->target > -360 )//屏幕最多显示5位
					pid_value->target = (pid_value->target - 1);
				break;		
		}	
	}
}