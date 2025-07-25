#include "mid_debug_led.h"

static DEBUG_LED_STRUCT debug_led;

// ��LED
void set_debug_led_on(void) 
{
	debug_led.state = LED_ON;
	DL_GPIO_setPins(DEBUG_LED_PORT, DEBUG_LED_PIN_22_PIN);
}

// �ر�LED
void set_debug_led_off(void) 
{
	debug_led.state = LED_OFF;
	DL_GPIO_clearPins(DEBUG_LED_PORT, DEBUG_LED_PIN_22_PIN);
}

// �л�LED״̬��������
void set_debug_led_toggle(void) 
{
	if( debug_led.state == LED_ON )
	{
		set_debug_led_off();
	}
	else if( debug_led.state == LED_OFF )
	{
		set_debug_led_on();
	}
}

// ��ȡLED��״̬
LED_STATE_ENUM get_debug_led_state(void)
{
	return debug_led.state;
}

