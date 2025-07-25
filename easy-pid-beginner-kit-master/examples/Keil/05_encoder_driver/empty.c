#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"
#include "stdio.h"
#include "hw_lcd.h"
#include "mid_button.h"
#include "app_key_task.h"
#include "hw_encoder.h"
#include "mid_timer.h"

void ui_home_page(void);// 首页页面初始化
void ui_encoder_value_update(void);//编码器的数据显示

int main(void)
{
		int sys_time = 0;
	
    SYSCFG_DL_init();
		
		//DEBUG串口初始化
		debug_uart_init();
		
		//按键任务初始化
    user_button_init();
		
		//编码器初始化
		encoder_init();
	
		//定时器初始化
		timer_init();
	
		//LCD初始化
		lcd_init();
		
		//LCD显示UI
		ui_home_page();
	
    while (1) 
		{
			sys_time++;

			if( sys_time % 20 == 0 )//2*20=40ms刷新一次屏幕
			{
					ui_encoder_value_update();
			}
			
			delay_cycles(CPUCLK_FREQ / 1000 * 2);//时间基准2ms
    }
}


/*
功能说明：显示居中字符串函数用于显示居中的文字，计算文字的居中坐标
参数说明：  x=屏幕中心x坐标
        w=屏幕宽度
        y=屏幕中心y坐标
        h=屏幕高度
        str_len=字符串长度
        sizey=字体大小
        *str=需要显示的字符串
        color背景颜色        
备注：GRAYBLUE 浅蓝色
      DARKBLUE 深蓝色
*/
void disp_x_center(int y, int str_len, uint16_t bc, unsigned char sizey, unsigned char* str)
{
    int str_center_x = (sizey * str_len) / 2;//字符串x=字体大小*字符串长度/2
    int str_center_y = sizey / 2;//字符串y=字体大小/2

    //显示居中坐标的文字    
	  LCD_ArcRect(screen_center_x - str_center_x - 10, y, screen_center_x + str_center_x + 10, sizey+y, bc);
    LCD_ShowChinese(screen_center_x - str_center_x,y,str,WHITE,bc,sizey,1);
}

/*
功能说明：显示字符串矩形函数用于显示矩形的文字，计算文字的居中坐标
参数说明：  x=矩形起始x坐标
        w=矩形宽度
        y=矩形起始y坐标
        h=矩形高度
        str_len=字符串长度
        sizey=字体大小
        *str=需要显示的字符串
        color背景颜色        
备注：GRAYBLUE 浅蓝色
      DARKBLUE 深蓝色
*/
void disp_string_rect(int x, int w, int y, int h, int str_len, int sizey, unsigned char* str, int color)
{
    int str_center_x = ((sizey/2) * str_len) / 2; //字符串x = 字体大小*字符串长度/2
    int rect_center_x = x + (w / 2); //矩形中心x
    int str_center_y = sizey  / 2; //字符串y=字体大小/2
    int rect_center_y = y + (h / 2); //矩形中心y

    //显示背景矩形    
	  LCD_ArcRect(x, y, x + w, y + h, color);
    //显示字符串
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}



//首页页面初始化
void ui_home_page(void)
{
    //关闭背光    
	  LCD_BLK_Clr();

    //显示全屏背景颜色
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    //显示标题
    disp_x_center(3,5,BLUE,16,(unsigned char *)"立创开发板");
    //显示副标题
    disp_x_center(3+16+3,8,BLUE,16,(unsigned char *)"简易PID入门套件");

    int x = 40;
    int x_offset = 90;
    int y = 65;
    int y_offset = 25;
    //显示speed
    disp_string_rect(x, x_offset, y, y_offset, 5, 24, "speed", BLUE);
    int x2 = 170;
    int x2_offset = 120;
    int y2 = 65;
    int y2_offset = 25;
    //显示direction
		disp_string_rect(x2, x2_offset, y2, y2_offset, 9, 24, "direction", BLUE);
	
    //打开背光   
		LCD_BLK_Set();
}

//编码器的速度+方向显示
void ui_encoder_value_update(void)
{
		static int last_encoder_count = 255;
		static int last_encoder_dir = 255;
		char* disp_buff[50]={0};
		
		if( last_encoder_count != get_encoder_count() )
		{
			last_encoder_count = get_encoder_count();
			sprintf(disp_buff, "%d", get_encoder_count() );
			
			int x = 30;
			int x_offset = 110;
			int y = 65+30;
			int y_offset = 25;
			disp_string_rect(x, x_offset, y, y_offset, strlen(disp_buff), 24, disp_buff, BLACK);
		}
		
		if( last_encoder_dir != get_encoder_dir()  )
		{
			last_encoder_dir = get_encoder_dir();
			int x2 = 170;
			int x2_offset = 120;
			int y2 = 65+30;
			int y2_offset = 25;
			sprintf(disp_buff, "%s", (get_encoder_dir() == FORWARD) ? "FORWARD" : "REVERSAL" );
			disp_string_rect(x2, x2_offset, y2, y2_offset, strlen(disp_buff), 24, disp_buff, BLACK);
		}
}
