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
static float last_value = 65535.0f;  // 设置为不可能的初始值
static char buff[16];                // 足够大的缓冲区

//TXT_OBJECT txt_value = {0};          // 结构体用于存储显示位置
//int char_width_pixel = 8;            // 英文字符宽度（8x16字库）
//int rect_x = 50;                     // 起始位置
//int rect_y = 80;
//int rect_w = 8 * 5;                  // 显示5个字符的宽度
//int rect_h = 16;                     // 字体高度

while (1)
{
    if (last_value != value)  // 数值变化时才更新显示
    {
        last_value = value;

        // 格式化为字符串
				sprintf(buff, "Num:%5.2f", value);  // 宽度5，保留两位小数
				LCD_ShowStr(20,20,buff,16);
    }

    // 数值递增
    value += 0.1f;
    if (value > 99.99f) value = 0.0f;

    delay_ms(10);
}

}

void delay_ms(uint32_t ms)
{
    // 这个是阻塞延时，精度不高，只适合简单测试
    for(uint32_t i = 0; i < ms * 4000; i++)
    {
        __NOP();  // 单周期空指令，耗时间
    }
}

