#include "app_key_task.h"

static char current_mode;

void set_app_key_current_mode(char mode)
{
	current_mode = mode;
}

char get_app_key_current_mode(void)
{
	return current_mode;
}

void btn_up_cb(flex_button_t *btn)
{ 
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件
					break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
					break;
        default:break;
    }
}

void btn_left_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件
					
						set_app_key_current_mode( !get_app_key_current_mode() );
				
            break;
				case FLEX_BTN_PRESS_LONG_START://长击开始事件
            break;
        default:break;
    }
}

void btn_right_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件  
					
						set_app_key_current_mode( !get_app_key_current_mode() );
				
            break;
        case FLEX_BTN_PRESS_LONG_START://长击开始事件
            break;
        default:break;
    }
}

void btn_down_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://单击事件            
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://长按保持事件 
						break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://长按保持后抬起事件 
						break;
        default:break;
    }
}
