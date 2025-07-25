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

#include <stdlib.h> //�����ʹ��

// ���ɷ�Χ�� 0 �� 88 ֮����������
int generate_random(void) 
{
	return (rand() % 89); 
}

int main(void)
{

	int sys_time = 0;

	SYSCFG_DL_init();

	//DEBUG���ڳ�ʼ��
	debug_uart_init();

	//���������ʼ��
	user_button_init();

	//��������ʼ��
	encoder_init();

	//��ʱ����ʼ��
	timer_init();

	//LCD��ʼ��
	lcd_init();

	//LCD��ʾUI
	ui_home_page();

	//��ͣ2��
	delay_cycles(80000000*2);

	//��ʾ���ٽ���
	ui_speed_page();
	//��ʾ���ٽ����PID����
	ui_speed_page_value_set(12.1, 2.2, 3.12, 50, 30, 1);
	//Iֵλ����ʾѡ���
	ui_speed_page_select_box(1);
	//Dֵλ����ʾѡ�п�
	ui_parameter_select_box_bold(2);

	while (1) 
	{
		sys_time++;

	if( sys_time % 10 == 0 )//2*10=20msˢ��һ����Ļ
	{
		//ʹ�������ˢ�²���
		draw_speed_curve(0, 0, 319, 88, GREEN, BLACK, generate_random() );  
	}

		delay_cycles(CPUCLK_FREQ / 1000 * 2);//ʱ���׼2ms
	}
}

