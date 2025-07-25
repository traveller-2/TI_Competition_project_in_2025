#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"

int main(void)
{
    SYSCFG_DL_init();
		
		debug_uart_init();
	
    while (1) 
		{
			debug_uart_send_string("run 01_template_project demo\r\n");
			
			if( strstr(get_debug_uart_receive_data(), "toggle led") != NULL)
			{
				set_debug_led_toggle();
				clear_debug_uart_receive_data();
			}
			
			delay_cycles(CPUCLK_FREQ / 1000 * 200);
    }
}
