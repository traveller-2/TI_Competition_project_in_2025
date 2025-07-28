/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "usart.h"
#include <string.h>
#include "Emm.h"
#include "string.h"
#include "hw_lcd.h"
#include "stdio.h"
#include "motorDriver.h"
#include "hw_encoder.h"
#include "mid_timer.h"

void delay_ms(uint32_t ms);

int left_cnt = 0;
int right_cnt = 0;

int main(void)
{
	SYSCFG_DL_init();
	
	//编码器初始化
	encoder_init();

	//定时器初始化
	timer_init();

	
	while(1)
	{

//			Motor_SetSpeed(10, 0); // 左正转60%，右反转40%
//			delay_ms(1000);			
//	
//			Motor_SetSpeed(20, 0); // 左正转60%，右反转40%
//			delay_ms(1000);			

//			Motor_SetSpeed(10, 0); // 左正转60%，右反转40%
//			delay_ms(1000);				

//			Motor_SetSpeed(-10, 0); // 左正转60%，右反转40%
//			delay_ms(1000);		
//			
//			Motor_SetSpeed(-50, 0); // 左正转60%，右反转40%
//			delay_ms(1000);		
//			
//			Motor_SetSpeed(0, 0); // 左正转60%，右反转40%
//			delay_ms(1000);		
	left_cnt = get_encoder_count_l();
	right_cnt = get_encoder_count_r();
	delay_ms(2000);	
	}
	

	
//		lcd_init();
//	
//		LCD_BLK_Set();
//		
//		LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
//		

//    int i = 0;
//		static char buff[16];                // 足够大的缓冲区
//		static float value = 0.0f;
//		static float last_value = 65535.0f;  // 设置为不可能的初始值
//    while (1)
//    {

//        // 呼吸灯渐亮过程
//        for (i = 0; i <= 999; i++)
//        {
//            // 设置 LED 亮度
//            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
//            delay_ms(1);  // 延迟以控制亮度变化速度

//				}
//        // 呼吸灯渐暗过程
//        for (i = 999; i > 0; i--)
//        {
//            // 设置 LED 亮度
//            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
//            delay_ms(1);  // 延迟以控制亮度变化速度
//        }
//				if(value !=last_value)
//				{
//					sprintf(buff,"Val:%5.2f",value);
//					LCD_ShowStr(32, 32, buff, 15);
//					value++;
//				}
//				
//				Emm_V5_Vel_Control(1,0,200,5,0);
//				delay_ms(1000);
//				const char *msg = "Hello MSPM0!\r\n";
//				UART_SendArray((uint8_t *)msg, strlen(msg));
//				delay_ms(1000);

//    }
}

void delay_ms(uint32_t ms)
{
    // 这个是阻塞延时，精度不高，只适合简单测试
    for(uint32_t i = 0; i < ms * 4000; i++)
    {
        __NOP();  // 单周期空指令，耗时间
    }
}
