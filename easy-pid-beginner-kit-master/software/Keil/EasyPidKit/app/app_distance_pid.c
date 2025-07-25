#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "stdlib.h"
#include "stdio.h"

PID distance_pid;

//����PID��ʼ��
void distance_pid_init(void)
{
    pid_init(&distance_pid, 75, 2, 10, 9999, 9999, DEFAULT_ANGLE );
}


//��ȡ����PIDȫ�ֱ����ĵ�ַ
PID* get_distance_pid(void)
{
	return &distance_pid;
}

//��ȡ����PID��Ŀ��ֵ
int get_distance_pid_target(void)
{
	return distance_pid.target;
}

/************************************************
���ܣ�PID������������
������target_speed = Ŀ��ֵ
���أ���ӦPID�ĵ�ַ
************************************************/
PID motor_distance_control(int target_angle)
{
	int PWM, target_pulses;
	// ��Ŀ��ǶȻ����Ŀ��������
	target_pulses = target_angle / DEGREES_PER_PULSE;
		
	//����PID��̬������Ŀ��ֵ��Ŀ��������������ǰֵ����ǰ��������ȡ��ʵʱ��������
	//PID�����ֵΪ�����źţ����뵽PWM������
	PWM = pid_calc( &distance_pid, target_pulses, get_temp_encoder() );
		
	//����ˢ���ٶ�
	delay_cycles(80000*5);
	
	//�����ź�תΪ���ʵ�ʿ���ֵ
	if( PWM > 0 )//�����źŴ���0
	{
		set_motor(0, PWM);//˳ʱ����ת��pwmΪ��ת�ٶ�
	}
	else if( PWM < 0 )
	{
		PWM = -PWM;//������תΪ����������PWM=-10����-��-10��=10��
		set_motor(PWM, 0);//��ʱ����ת��pwmΪ��ת�ٶ�
	}
	return distance_pid;
}

/************************************************
���ܣ����ö���PID�Ĳ�������
������pid_value = ��ӦPID�ĵ�ַ
	  select = 0��ʾ����P��= 1��ʾ����I��= 2��ʾ����D��= 3��ʾ����target
	  add_or_subtract_flag = 0��ʾ�ӣ�=1��ʾ��
************************************************/
void set_distance_pid_parameter(PID* pid_value, int select, int add_or_subtract_flag)
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
				if( pid_value->target < 360 )
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
					pid_value->kp = pid_value->kp - 0.1f;
				pid_value->kp = (pid_value->kp <= -0.0f) ? 0.0f : pid_value->kp;  // ��������
				break;
			case 1://����I
				if( pid_value->ki > 0 )
					pid_value->ki = pid_value->ki - 0.1f;
				pid_value->ki = (pid_value->ki <= -0.0f) ? 0.0f : pid_value->ki;  // ��������
				break;
			case 2://����D
				if( pid_value->kd > 0 )
					pid_value->kd = pid_value->kd - 0.1f;
				pid_value->kd = (pid_value->kd <= -0.0f) ? 0.0f : pid_value->kd;  // ��������
				break;			
			case 3://����target
				if( pid_value->target > -360 )//��Ļ�����ʾ5λ
					pid_value->target = (pid_value->target - 1);
				break;		
		}	
	}
}