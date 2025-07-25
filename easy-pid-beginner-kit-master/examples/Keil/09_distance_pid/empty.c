#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"
#include "stdio.h"
#include "hw_lcd.h"
#include "mid_button.h"
#include "app_key_task.h"
#include "hw_encoder.h"
#include "mid_timer.h"
#include "app_ui.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "hw_motor.h"
#include "app_distance_pid.h"

void ui_speed_or_distance_page_value_set_quick(LongPressStatus update_status)
{
    if( update_status == LONG_PRESS_END )
        return;

    if(update_status == LONG_PRESS_ADD_START )
    {
        if( get_functional_mode() == SPEED_FUNCTION )
            set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 0);
        else if( get_functional_mode() == DISTANCE_FUNCTION )
            set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 0);
    }
    else if( update_status == LONG_PRESS_SUBTRACT_START )   
    {
        if( get_functional_mode() == SPEED_FUNCTION )
            set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 1);
        else if( get_functional_mode() == DISTANCE_FUNCTION )
            set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 1);
    }
}

int main(void)
{
	SYSCFG_DL_init();

	//DEBUG���ڳ�ʼ��
	debug_uart_init();

	//���������ʼ��
	user_button_init();

	//��������ʼ��
	encoder_init();

	//��ʱ����ʼ��
	timer_init();

	//ϵͳ������ʼ��
	sys_event_init();
	
	//����PID��ʼ��
    speed_pid_init();
	
	//����PID��ʼ��
	distance_pid_init();
	
	//LCD��ʼ��
	lcd_init();

	//LCD��ʾUI
	ui_home_page();

	while (1) 
	{
		if( get_motor_status_flag() == MOTOR_STATUS_ON )
        {
            switch ( get_functional_mode() ) 
            {
                case SPEED_FUNCTION:

					//���㶨��PID
					motor_speed_control( get_speed_pid_target() );
					
					//��ֹ�����ͻ�����ж�һ��
					if( get_motor_status_flag() == MOTOR_STATUS_ON )
					{
						//��ʾPID���κͲ���
						ui_speed_curve();
					}
					else if( get_motor_status_flag() == MOTOR_STATUS_OFF )
					{
						//ֹͣ���
						stop_motor();
						pid_change_zero(get_speed_pid());
					}
                break;

                case DISTANCE_FUNCTION:
					
					//���㶨��PID
					motor_distance_control( get_distance_pid_target() );
					//��ֹ�����ͻ�����ж�һ��
					if( get_motor_status_flag() == MOTOR_STATUS_ON )
					{
						//��ʾPID���κͲ���
						ui_distance_curve();
					}
					else if( get_motor_status_flag() == MOTOR_STATUS_OFF )
					{
						//ֹͣ���
						stop_motor();
						pid_change_zero(get_distance_pid());
					}
				
                break;
                default:
					enable_task_interrupt(); //�����������
					pid_change_zero(get_speed_pid());
					set_motor_status_flag( MOTOR_STATUS_OFF );
                break;
            }
        }
		
		if( get_show_state() == PARAMETER_PAGE )
        {
			//��ֵ�Ŀ��ټӼ�
			ui_speed_or_distance_page_value_set_quick( get_long_press_state() );
			
            //��Ļ��ʾ�����仯
            PID* temp_pid = (get_functional_mode() == SPEED_FUNCTION) ?  get_speed_pid() : get_distance_pid();
            int encoder_value = (get_functional_mode() == SPEED_FUNCTION) ? get_encoder_count() : (get_temp_encoder() * DEGREES_PER_PULSE);           
            ui_speed_page_value_set(temp_pid->kp, temp_pid->ki, temp_pid->kd, encoder_value, temp_pid->target, 1);
        }
	}
}

