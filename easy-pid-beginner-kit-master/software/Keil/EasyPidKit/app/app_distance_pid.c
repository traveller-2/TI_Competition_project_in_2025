#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "stdlib.h"
#include "stdio.h"

PID distance_pid;

//定距PID初始化
void distance_pid_init(void)
{
    pid_init(&distance_pid, 75, 2, 10, 9999, 9999, DEFAULT_ANGLE );
}


//获取定距PID全局变量的地址
PID* get_distance_pid(void)
{
	return &distance_pid;
}

//获取定距PID的目标值
int get_distance_pid_target(void)
{
	return distance_pid.target;
}

/************************************************
功能：PID电机定距控制器
参数：target_speed = 目标值
返回：对应PID的地址
************************************************/
PID motor_distance_control(int target_angle)
{
	int PWM, target_pulses;
	// 将目标角度换算成目标脉冲数
	target_pulses = target_angle / DEGREES_PER_PULSE;
		
	//传入PID静态参数、目标值（目标脉冲数）、当前值（当前编码器获取的实时脉冲数）
	//PID输出的值为控制信号，传入到PWM变量中
	PWM = pid_calc( &distance_pid, target_pulses, get_temp_encoder() );
		
	//控制刷新速度
	delay_cycles(80000*5);
	
	//控制信号转为电机实际控制值
	if( PWM > 0 )//控制信号大于0
	{
		set_motor(0, PWM);//顺时针旋转，pwm为旋转速度
	}
	else if( PWM < 0 )
	{
		PWM = -PWM;//将负数转为正数（例如PWM=-10，则-（-10）=10）
		set_motor(PWM, 0);//逆时针旋转，pwm为旋转速度
	}
	return distance_pid;
}

/************************************************
功能：设置定距PID的参数更新
参数：pid_value = 对应PID的地址
	  select = 0表示调整P，= 1表示调整I，= 2表示调整D，= 3表示调整target
	  add_or_subtract_flag = 0表示加，=1表示减
************************************************/
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