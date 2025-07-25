#include "app_key_task.h"
#include "app_ui.h"
#include "hw_motor.h"
#include "hw_encoder.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"


void btn_up_cb(flex_button_t *btn)
{ 
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�
			if( get_show_state() == DEFAULT_PAGE )//��ǰ����ҳ
            {
                system_status.default_page_flag = ( system_status.default_page_flag + 1 ) % 2;
                ui_home_page_select(system_status.default_page_flag);//��ʾ��ҳѡ���
                
            }
            if( get_show_state() == SET_PAGE )//��ǰ������ҳ
            {
                system_status.set_page_flag--;
                if( system_status.set_page_flag < 0 ) system_status.set_page_flag = 3;
                ui_speed_page_select_box(system_status.set_page_flag);//��ʾѡ���
                
            }
            if( get_show_state() == PARAMETER_PAGE )//��ǰ�ǲ���ҳ
            {
                if( get_functional_mode() == SPEED_FUNCTION )//���֮ǰ�Ƕ��ٹ���
				{
					//���ٲ������� û����ʾ
					set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 0);
                    
                }
				else if( get_functional_mode() == DISTANCE_FUNCTION )//����Ƕ��๦��
                {
					//����������� û����ʾ
					set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 0);
				}
			}
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://���������¼�
			//�����������¼�
			event_manager(&system_status, LONG_PRESS_ADD_START_EVENT);
			break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://�������ֺ�̧���¼� 
			//��������ֹͣ�¼�
            event_manager(&system_status, LONG_PRESS_END_EVENT);
			break;
        default:break;
    }
}

void btn_left_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�
			//�����ǰ�Ƕ��ٻ��߶���ҳ
			if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//��ʾ��ҳ
                ui_select_page_show(2);
				//���״̬����Ϊֹͣ
                set_motor_status_flag(MOTOR_STATUS_OFF);
                //ֹͣ���
				stop_motor();
                //�����˳��¼�
				event_manager(&system_status, QUIT_EVENT);
            }
			//�����ǰ������ҳ
            if( get_show_state() == SET_PAGE )
            {
				//�����˳��¼�
                event_manager(&system_status, QUIT_EVENT);
				//����ȫ��ѡ���
                ui_speed_page_select_box(4);
            }
			//�����ǰ�ǵ���ҳ
            if( get_show_state() == PARAMETER_PAGE )
            {
				//�����˳��¼�
                event_manager(&system_status, QUIT_EVENT);
                //��ʾ����ҳ��ѡ���
				ui_parameter_select_box_bold( get_set_page_flag() + 4 );
            }		
            break;
				
        default:break;
    }
}

void btn_right_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�  
			//�������ҳ
			if( get_show_state() == DEFAULT_PAGE )
            {
				//���������¼�
                event_manager(&system_status, ENTER_EVENT);
                //������ҳѡ����ʾ��Ӧ����ҳ
				ui_select_page_show( get_default_page_flag() );
                //�����һ���Ƕ���ҳ
				if( get_show_state() == PID_PAGE )
				{
					//��ʾ����ҳ�Ĳ���
					ui_speed_page_value_set(get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd, get_encoder_count(), get_speed_pid()->target, 0);
                }
				//�����һ���Ƕ���ҳ
				if( get_show_state() == DISTANCE_PAGE )
                {
					//��ʾ����ҳ�Ĳ���
					int current_angle = get_temp_encoder() * DEGREES_PER_PULSE;
					ui_distance_page_value_set(get_distance_pid()->kp, get_distance_pid()->ki, get_distance_pid()->kd, current_angle, get_distance_pid()->target, 0);
                }
            }
			//����Ƕ���ҳ���߶���ҳ
            else if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//���������¼�
                event_manager(&system_status, ENTER_EVENT);
				//��ʾѡ���
                ui_speed_page_select_box(system_status.set_page_flag);
            }
			//���������ҳ
            else if( get_show_state() == SET_PAGE )
            {
				//���������¼�
                event_manager(&system_status, ENTER_EVENT);
                //��ʾѡ�п�
				ui_parameter_select_box_bold(system_status.set_page_flag);
            }		
            break;
        case FLEX_BTN_PRESS_LONG_START://������ʼ�¼�
			//����Ƕ���ҳ���߶���ҳ
            if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//��������¼�
                event_manager(&system_status, MOTOR_EVENT);
                //������״̬�ǹ�
				if( get_motor_status_flag() == MOTOR_STATUS_OFF )
                {
					//ֹͣ���
                    stop_motor();
                }
            }
            break;
        default:break;
    }
}

void btn_down_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�          
			//�������ҳ
			if( get_show_state() == DEFAULT_PAGE )
            {
                system_status.default_page_flag--;
                if( system_status.default_page_flag < 0 ) system_status.default_page_flag = 1;
                ui_home_page_select(system_status.default_page_flag);//��ʾѡ���
            }
			//���������ҳ
            if( get_show_state() == SET_PAGE )
            {
                system_status.set_page_flag = ( system_status.set_page_flag + 1 ) % 4;
                ui_speed_page_select_box(system_status.set_page_flag);//��ʾѡ���
            }
			//����ǵ���ҳ
            if( get_show_state() == PARAMETER_PAGE )
            {
                if( get_functional_mode() == SPEED_FUNCTION )//���֮ǰ�Ƕ��ٹ���
				{
					//���ٲ������� û����ʾ
					set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 1);
				}
                else if( get_functional_mode() == DISTANCE_FUNCTION )
				{
					//����������� û����ʾ
					set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 1);
				}
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://���������¼� 
			//�����������¼�
			event_manager(&system_status, LONG_PRESS_SUBTRACT_START_EVENT);
			break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://�������ֺ�̧���¼� 
			//�������������¼�
			event_manager(&system_status, LONG_PRESS_END_EVENT);
			break;
        default:break;
    }
}
