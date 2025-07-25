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
        case FLEX_BTN_PRESS_CLICK://�����¼�
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://���������¼�
					break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://�������ֺ�̧���¼� 
					break;
        default:break;
    }
}

void btn_left_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�
					
						set_app_key_current_mode( !get_app_key_current_mode() );
				
            break;
				case FLEX_BTN_PRESS_LONG_START://������ʼ�¼�
            break;
        default:break;
    }
}

void btn_right_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�  
					
						set_app_key_current_mode( !get_app_key_current_mode() );
				
            break;
        case FLEX_BTN_PRESS_LONG_START://������ʼ�¼�
            break;
        default:break;
    }
}

void btn_down_cb(flex_button_t *btn)
{
    switch (btn->event)
    {
        case FLEX_BTN_PRESS_CLICK://�����¼�            
            break;
        case FLEX_BTN_PRESS_LONG_HOLD://���������¼� 
						break;
        case FLEX_BTN_PRESS_LONG_HOLD_UP://�������ֺ�̧���¼� 
						break;
        default:break;
    }
}
