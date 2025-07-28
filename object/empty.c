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
#include "motorMonitor.h"

void delay_ms(uint32_t ms);

float left_cnt = 0;
float right_cnt = 0;

int main(void)
{
	SYSCFG_DL_init();
	
	//��������ʼ��
	encoder_init();

	//��ʱ����ʼ��
	timer_init();

	lcd_init();

	LCD_BLK_Set();
	
	LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
	

	int i = 0;
	static char buff[16];                // �㹻��Ļ�����
	static float value = 0.0f;
	static float last_value = 65535.0f;  // ����Ϊ�����ܵĳ�ʼֵ

	while(1)
	{

//			Motor_SetSpeed(10, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);			
//	
//			Motor_SetSpeed(20, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);			

//			Motor_SetSpeed(10, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);				

//			Motor_SetSpeed(-10, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);		
//			
//			Motor_SetSpeed(-50, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);		
//			
//			Motor_SetSpeed(0, 0); // ����ת60%���ҷ�ת40%
//			delay_ms(1000);		
		left_cnt = get_output_rpm_l();
		right_cnt = get_output_rpm_r();
			
			if(value !=last_value)
		{
			sprintf(buff,"Val:%5.2f",left_cnt);
			LCD_ShowStr(32, 32, buff, 15);
			delay_ms(5);
			sprintf(buff,"Val:%5.2f",right_cnt);
			LCD_ShowStr(32, 64, buff, 15);			
		}
				
	delay_ms(50);	
	}
	

	


//    while (1)
//    {

//        // �����ƽ�������
//        for (i = 0; i <= 999; i++)
//        {
//            // ���� LED ����
//            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
//            delay_ms(1);  // �ӳ��Կ������ȱ仯�ٶ�

//				}
//        // �����ƽ�������
//        for (i = 999; i > 0; i--)
//        {
//            // ���� LED ����
//            DL_TimerG_setCaptureCompareValue(PWM_LED_INST,i,GPIO_PWM_LED_C1_IDX);
//            delay_ms(1);  // �ӳ��Կ������ȱ仯�ٶ�
//        }

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
    // �����������ʱ�����Ȳ��ߣ�ֻ�ʺϼ򵥲���
    for(uint32_t i = 0; i < ms * 4000; i++)
    {
        __NOP();  // �����ڿ�ָ���ʱ��
    }
}
