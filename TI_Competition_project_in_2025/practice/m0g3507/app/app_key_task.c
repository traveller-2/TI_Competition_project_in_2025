#include "app_key_task.h"
#include "app_ui.h"
#include "hw_motor.h"
#include "hw_encoder.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"


void btn_up_cb(flex_button_t *btn)
{ 
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件
			if( get_show_state() == DEFAULT_PAGE )//当前是首页
            {
                system_status.default_page_flag = ( system_status.default_page_flag + 1 ) % 2;
                ui_home_page_select(system_status.default_page_flag);//显示首页选择框
                
            }
            if( get_show_state() == SET_PAGE )//当前是设置页
            {
                system_status.set_page_flag--;
                if( system_status.set_page_flag < 0 ) system_status.set_page_flag = 3;
                ui_speed_page_select_box(system_status.set_page_flag);//显示选择框
                
            }
            if( get_show_state() == PARAMETER_PAGE )//当前是参数页
            {
                if( get_functional_mode() == SPEED_FUNCTION )//如果之前是定速功能
				{
					//定速参数更新 没有显示
					set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 0);
                    
                }
				else if( get_functional_mode() == DISTANCE_FUNCTION )//如果是定距功能
                {
					//定距参数显示
					ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
				}
			}
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件
			//触发长按加事件
			event_manager(&system_status, LONG_PRESS_ADD_START_EVENT);
			break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
			//触发长按停止事件
            event_manager(&system_status, LONG_PRESS_END_EVENT);
			break;
        default:break;
    }
}

void btn_left_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件
			//如果当前是定速或者定距页
			if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//显示首页
                ui_select_page_show(2);
				//电机状态设置为停止
                set_motor_status_flag(MOTOR_STATUS_OFF);
                //停止电机
				stop_motor();
                //触发退出事件
				event_manager(&system_status, QUIT_EVENT);
            }
			//如果当前是设置页
            if( get_show_state() == SET_PAGE )
            {
				//触发退出事件
                event_manager(&system_status, QUIT_EVENT);
				//擦除全部选择框
                ui_speed_page_select_box(4);
            }
			//如果当前是调参页
            if( get_show_state() == PARAMETER_PAGE )
            {
				//触发退出事件
                event_manager(&system_status, QUIT_EVENT);
                //显示设置页的选择框
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
        case FLEX_BTN_PRESS_CLICK://单击事件  
			//如果是首页
			if( get_show_state() == DEFAULT_PAGE )
            {
				//触发进入事件
                event_manager(&system_status, ENTER_EVENT);
                //根据首页选项显示对应功能页
				ui_select_page_show( get_default_page_flag() );
                //如果下一个是定速页
				if( get_show_state() == PID_PAGE )
				{
					//显示定速页的参数
					ui_speed_page_value_set(get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd, get_encoder_count(), get_speed_pid()->target, 0);
                }
				//如果下一个是定距页
				if( get_show_state() == DISTANCE_PAGE )
                {
					//显示定距页的参数
					ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
                }
            }
			//如果是定速页或者定距页
            else if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//触发进入事件
                event_manager(&system_status, ENTER_EVENT);
				//显示选择框
                ui_speed_page_select_box(system_status.set_page_flag);
            }
			//如果是设置页
            else if( get_show_state() == SET_PAGE )
            {
				//触发进入事件
                event_manager(&system_status, ENTER_EVENT);
                //显示选中框
				ui_parameter_select_box_bold(system_status.set_page_flag);
            }		
            break;
        case FLEX_BTN_PRESS_LONG_START://长击开始事件
			//如果是定速页或者定距页
            if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
				//触发电机事件
                event_manager(&system_status, MOTOR_EVENT);
                //如果电机状态是关
				if( get_motor_status_flag() == MOTOR_STATUS_OFF )
                {
					//停止电机
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
        case FLEX_BTN_PRESS_CLICK://单击事件          
			//如果是首页
			if( get_show_state() == DEFAULT_PAGE )
            {
                system_status.default_page_flag--;
                if( system_status.default_page_flag < 0 ) system_status.default_page_flag = 1;
                ui_home_page_select(system_status.default_page_flag);//显示选择框
            }
			//如果是设置页
            if( get_show_state() == SET_PAGE )
            {
                system_status.set_page_flag = ( system_status.set_page_flag + 1 ) % 4;
                ui_speed_page_select_box(system_status.set_page_flag);//显示选择框
            }
			//如果是调参页
            if( get_show_state() == PARAMETER_PAGE )
            {
                if( get_functional_mode() == SPEED_FUNCTION )//如果之前是定速功能
				{
					//定速参数更新 没有显示
					set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 1);
				}
                else if( get_functional_mode() == DISTANCE_FUNCTION )
				{
					//定距参数显示
					ui_distance_page_value_set(12.12, 11.123, 1.223, 90, 90, 0);
				}
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件 
			//触发长按减事件
			event_manager(&system_status, LONG_PRESS_SUBTRACT_START_EVENT);
			break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
			//触发长按结束事件
			event_manager(&system_status, LONG_PRESS_END_EVENT);
			break;
        default:break;
    }
}
