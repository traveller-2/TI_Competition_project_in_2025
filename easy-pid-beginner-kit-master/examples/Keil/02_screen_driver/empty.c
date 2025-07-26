#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"
#include "hw_lcd.h"
#include "stdio.h"
#include "string.h"

void delay_ms(uint32_t ms);

typedef struct {
    int start_x;
    int start_y;
} TXT_OBJECT;

int main(void)
{
    SYSCFG_DL_init();
			
		lcd_init();
	
		LCD_BLK_Set();
		
		LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);

static float value = 0.0f;
static float last_value = 65535.0f;  // ����Ϊ�����ܵĳ�ʼֵ
static char buff[16];                // �㹻��Ļ�����

//TXT_OBJECT txt_value = {0};          // �ṹ�����ڴ洢��ʾλ��
//int char_width_pixel = 8;            // Ӣ���ַ���ȣ�8x16�ֿ⣩
//int rect_x = 50;                     // ��ʼλ��
//int rect_y = 80;
//int rect_w = 8 * 5;                  // ��ʾ5���ַ��Ŀ��
//int rect_h = 16;                     // ����߶�

while (1)
{
    if (last_value != value)  // ��ֵ�仯ʱ�Ÿ�����ʾ
    {
        last_value = value;

        // ��ʽ��Ϊ�ַ���
				sprintf(buff, "Num:%5.2f", value);  // ���5��������λС��
				LCD_ShowStr(20,20,buff,16);
    }

    // ��ֵ����
    value += 0.1f;
    if (value > 99.99f) value = 0.0f;

    delay_ms(10);
}

}

void delay_ms(uint32_t ms)
{
    // �����������ʱ�����Ȳ��ߣ�ֻ�ʺϼ򵥲���
    for(uint32_t i = 0; i < ms * 4000; i++)
    {
        __NOP();  // �����ڿ�ָ���ʱ��
    }
}

