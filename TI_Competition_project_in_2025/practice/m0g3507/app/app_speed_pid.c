#include "app_speed_pid.h"

#include "hw_encoder.h"
#include "hw_motor.h"


PID speed_pid;

//定速PID初始化
void speed_pid_init(void)
{
    pid_init(&speed_pid, 35, 6, 10, MOTOR_PWM_MAX, MOTOR_PWM_MAX,98);
}

//获取定速PID全局变量的地址
PID* get_speed_pid(void)
{
	return &speed_pid;
}

//获取定速PID的目标值
int get_speed_pid_target(void)
{
	return speed_pid.target;
}

/************************************************
功能：PID电机定速控制器
参数：target_speed = 目标值
返回：对应PID的地址
************************************************/
PID motor_speed_control(int target_speed)
{

	int PWM;

	PWM = pid_calc(&speed_pid, target_speed, get_encoder_count() );
	
	//控制刷新速度
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

/************************************************
功能：设置速度PID的参数更新
参数：pid_value = 对应PID的地址
	  select = 0表示调整P，= 1表示调整I，= 2表示调整D，= 3表示调整target
	  add_or_subtract_flag = 0表示加，=1表示减
************************************************/
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