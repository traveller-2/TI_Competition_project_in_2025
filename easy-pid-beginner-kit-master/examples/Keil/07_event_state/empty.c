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

#define SPEED_ENCODER_VALUE_MAX ( -(SPEED_ENCODER_VALUE_MIN) )
#define SPEED_ENCODER_VALUE_MID 0
#define SPEED_ENCODER_VALUE_MIN (-100)

#define DISTANCE_ENCODER_VALUE_MAX ( -(DISTANCE_ENCODER_VALUE_MIN) )
#define DISTANCE_ENCODER_VALUE_MID 0
#define DISTANCE_ENCODER_VALUE_MIN (-360)


int main(void)
{
	int curve_x = 0;
	
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

                //��ֹ�����ͻ�����ж�һ��
                if( get_motor_status_flag() == MOTOR_STATUS_ON )
                {
					
					if( curve_x < 319 ) curve_x++;
					
					//����Ŀ���ٶȵĲ��ε�
					LCD_DrawPoint(curve_x, 88 - ((50 + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);

                }
                break;

                case DISTANCE_FUNCTION:
					
                //��ֹ�����ͻ�����ж�һ��
                if( get_motor_status_flag() == MOTOR_STATUS_ON )
                {
                    if( curve_x > 0  ) curve_x--;
					
					//����Ŀ���ٶȵĲ��ε�
					LCD_DrawPoint(curve_x, 88 - ((50 + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);
                }
                break;
            }
        }
		
		if( get_show_state() == PARAMETER_PAGE )
        {
            //��������ʱ����Ļ��ʾ�����仯
            //ui_speed_page_value_set(temp_pid->kp, temp_pid->ki, temp_pid->kd, encoder_value, temp_pid->target, 1);
        }
	}
}

