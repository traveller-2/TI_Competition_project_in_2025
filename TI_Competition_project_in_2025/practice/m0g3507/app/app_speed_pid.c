#include "app_speed_pid.h"

#include "hw_encoder.h"
#include "hw_motor.h"


PID speed_pid;

//����PID��ʼ��
void speed_pid_init(void)
{
    pid_init(&speed_pid, 35, 6, 10, MOTOR_PWM_MAX, MOTOR_PWM_MAX,98);
}

//��ȡ����PIDȫ�ֱ����ĵ�ַ
PID* get_speed_pid(void)
{
	return &speed_pid;
}

//��ȡ����PID��Ŀ��ֵ
int get_speed_pid_target(void)
{
	return speed_pid.target;
}

/************************************************
���ܣ�PID������ٿ�����
������target_speed = Ŀ��ֵ
���أ���ӦPID�ĵ�ַ
************************************************/
PID motor_speed_control(int target_speed)
{

	int PWM;

	PWM = pid_calc(&speed_pid, target_speed, get_encoder_count() );
	
	//����ˢ���ٶ�
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
���ܣ������ٶ�PID�Ĳ�������
������pid_value = ��ӦPID�ĵ�ַ
	  select = 0��ʾ����P��= 1��ʾ����I��= 2��ʾ����D��= 3��ʾ����target
	  add_or_subtract_flag = 0��ʾ�ӣ�=1��ʾ��
************************************************/
void set_speed_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag)
{
	if( add_or_subtract_flag == 0 ) //��
	{
		switch(select)
		{
			case 0://����P
				pid_value->kp = pid_value->kp + 0.1;
				break;
			case 1://����I
				pid_value->ki = pid_value->ki + 0.1;
				break;
			case 2://����D
				pid_value->kd = pid_value->kd + 0.1;
				break;			
			case 3://����target
				if( pid_value->target < 100 )
					pid_value->target = pid_value->target + 1;
				break;		
		}
	} 
	else //��
	{
		switch(select)
		{
			case 0://����P
				if(  pid_value->kp > 0 )
					pid_value->kp = pid_value->kp - 0.1;
				pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp;  // ��������
				break;
			case 1://����I
				if( pid_value->ki > 0 )
					pid_value->ki = pid_value->ki - 0.1;
				pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki;  // ��������
				break;
			case 2://����D
				if( pid_value->kd > 0 )
					pid_value->kd = pid_value->kd - 0.1;
				pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd;  // ��������
				break;			
			case 3://����target
				if( pid_value->target > -100 )
					pid_value->target = (pid_value->target - 1);
				break;		
		}	
	}
}