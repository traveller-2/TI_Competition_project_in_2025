#include "app_key_task.h"
#include "hw_encoder.h"
#include "hw_motor.h"
#include "app_ui.h"
#include "app_sys_mode.h"
#include "mid_pid.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include <stdio.h>

#define 	delay_ms(X) 	delay_cycles( ( 80000 * (X) ) )



//DL_GPIO_togglePins(SYS_LED_PORT,SYS_LED_PIN_22_PIN);
void btn_up_cb(flex_button_t *btn)
{ 
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件            
            if( get_show_state() == DEFAULT_PAGE )
            {
                system_status.default_page_flag = ( system_status.default_page_flag + 1 ) % 2;
                ui_home_page_select(system_status.default_page_flag);
                
            }
            if( get_show_state() == SET_PAGE )
            {
                system_status.set_page_flag--;
                if( system_status.set_page_flag < 0 ) system_status.set_page_flag = 3;
                ui_speed_page_select_box(system_status.set_page_flag);//显示选择框
                
            }
            if( get_show_state() == PARAMETER_PAGE )
            {
                if( get_functional_mode() == SPEED_FUNCTION )//如果之前是定速功能
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 0);
                else if( get_functional_mode() == DISTANCE_FUNCTION )
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 0);
            }
            break;
            
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件 
            event_manager(&system_status, LONG_PRESS_ADD_START_EVENT);
        break;

        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
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
            if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
                ui_select_page_show(2);
                stop_motor();
                set_motor_status_flag(MOTOR_STATUS_OFF);
                event_manager(&system_status, QUIT_EVENT);
            }
            if( get_show_state() == SET_PAGE )
            {
                event_manager(&system_status, QUIT_EVENT);
                ui_speed_page_select_box(4);//擦除选择框
            }
            if( get_show_state() == PARAMETER_PAGE )
            {
                event_manager(&system_status, QUIT_EVENT);
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
            if( get_show_state() == DEFAULT_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_select_page_show( get_default_page_flag() );
                if( get_show_state() == PID_PAGE )
                    ui_speed_page_value_set(get_speed_pid()->kp,get_speed_pid()->ki,get_speed_pid()->kd,
                                            get_encoder(),get_speed_pid()->target, 0);
                if( get_show_state() == DISTANCE_PAGE )
                {
                    ui_distance_page_value_set(get_distance_pid()->kp,get_distance_pid()->ki,get_distance_pid()->kd,
                                            get_temp_encoder(),get_distance_pid()->target, 0);
                }
            }
            else if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_speed_page_select_box(system_status.set_page_flag);
            }            
            else if( get_show_state() == SET_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_parameter_select_box_bold(system_status.set_page_flag);
            }
            break;
        case FLEX_BTN_PRESS_LONG_START://长击开始事件
            
            if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
                //set_motor_status_toggle();
                event_manager(&system_status, MOTOR_EVENT);
                if( get_motor_status_flag() == MOTOR_STATUS_OFF )
                {
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
            if( get_show_state() == DEFAULT_PAGE )
            {
                system_status.default_page_flag--;
                if( system_status.default_page_flag < 0 ) system_status.default_page_flag = 1;
                ui_home_page_select(system_status.default_page_flag);//显示选择框
            }
            if( get_show_state() == SET_PAGE )
            {
                system_status.set_page_flag = ( system_status.set_page_flag + 1 ) % 4;
                ui_speed_page_select_box(system_status.set_page_flag);//显示选择框
            }
            if( get_show_state() == PARAMETER_PAGE )
            {
                if( get_functional_mode() == SPEED_FUNCTION )//如果之前是定速功能
                    set_speed_pid_parameter(get_speed_pid(), system_status.set_page_flag, 1);
                else if( get_functional_mode() == DISTANCE_FUNCTION )
                    set_distance_pid_parameter(get_distance_pid(), system_status.set_page_flag, 1);
            }
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件 
            event_manager(&system_status, LONG_PRESS_SUBTRACT_START_EVENT);
            
        break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
            event_manager(&system_status, LONG_PRESS_END_EVENT);
        break;
        default:break;
    }
}

void btn_mid_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件        
            if( get_show_state() == DEFAULT_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_select_page_show( get_default_page_flag() );
                if( get_show_state() == PID_PAGE )
                    ui_speed_page_value_set(get_speed_pid()->kp,get_speed_pid()->ki,get_speed_pid()->kd,
                                            get_encoder(),get_speed_pid()->target, 0);
                if( get_show_state() == DISTANCE_PAGE )
                {
                    ui_distance_page_value_set(get_distance_pid()->kp,get_distance_pid()->ki,get_distance_pid()->kd,
                                            get_temp_encoder(),get_distance_pid()->target, 0);
                }
            }
            else if( get_show_state() == PID_PAGE || get_show_state() == DISTANCE_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_speed_page_select_box(system_status.set_page_flag);
            }            
            else if( get_show_state() == SET_PAGE )
            {
                event_manager(&system_status, ENTER_EVENT);
                ui_parameter_select_box_bold(system_status.set_page_flag);
            }    
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件 
        break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
        break;
        default:break;
    }
}