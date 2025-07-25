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

void ui_home_page(void);// ��ҳҳ���ʼ��
void ui_encoder_value_update(void);//��������������ʾ

int main(void)
{
		int sys_time = 0;
	
    SYSCFG_DL_init();
		
		//DEBUG���ڳ�ʼ��
		debug_uart_init();
		
		//���������ʼ��
    user_button_init();
		
		//��������ʼ��
		encoder_init();
	
		//��ʱ����ʼ��
		timer_init();
	
		//LCD��ʼ��
		lcd_init();
		
		//LCD��ʾUI
		ui_home_page();
	
    while (1) 
		{
			sys_time++;

			if( sys_time % 20 == 0 )//2*20=40msˢ��һ����Ļ
			{
					ui_encoder_value_update();
			}
			
			delay_cycles(CPUCLK_FREQ / 1000 * 2);//ʱ���׼2ms
    }
}


/*
����˵������ʾ�����ַ�������������ʾ���е����֣��������ֵľ�������
����˵����  x=��Ļ����x����
        w=��Ļ���
        y=��Ļ����y����
        h=��Ļ�߶�
        str_len=�ַ�������
        sizey=�����С
        *str=��Ҫ��ʾ���ַ���
        color������ɫ        
��ע��GRAYBLUE ǳ��ɫ
      DARKBLUE ����ɫ
*/
void disp_x_center(int y, int str_len, uint16_t bc, unsigned char sizey, unsigned char* str)
{
    int str_center_x = (sizey * str_len) / 2;//�ַ���x=�����С*�ַ�������/2
    int str_center_y = sizey / 2;//�ַ���y=�����С/2

    //��ʾ�������������    
	  LCD_ArcRect(screen_center_x - str_center_x - 10, y, screen_center_x + str_center_x + 10, sizey+y, bc);
    LCD_ShowChinese(screen_center_x - str_center_x,y,str,WHITE,bc,sizey,1);
}

/*
����˵������ʾ�ַ������κ���������ʾ���ε����֣��������ֵľ�������
����˵����  x=������ʼx����
        w=���ο��
        y=������ʼy����
        h=���θ߶�
        str_len=�ַ�������
        sizey=�����С
        *str=��Ҫ��ʾ���ַ���
        color������ɫ        
��ע��GRAYBLUE ǳ��ɫ
      DARKBLUE ����ɫ
*/
void disp_string_rect(int x, int w, int y, int h, int str_len, int sizey, unsigned char* str, int color)
{
    int str_center_x = ((sizey/2) * str_len) / 2; //�ַ���x = �����С*�ַ�������/2
    int rect_center_x = x + (w / 2); //��������x
    int str_center_y = sizey  / 2; //�ַ���y=�����С/2
    int rect_center_y = y + (h / 2); //��������y

    //��ʾ��������    
	  LCD_ArcRect(x, y, x + w, y + h, color);
    //��ʾ�ַ���
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}



//��ҳҳ���ʼ��
void ui_home_page(void)
{
    //�رձ���    
	  LCD_BLK_Clr();

    //��ʾȫ��������ɫ
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    //��ʾ����
    disp_x_center(3,5,BLUE,16,(unsigned char *)"����������");
    //��ʾ������
    disp_x_center(3+16+3,8,BLUE,16,(unsigned char *)"����PID�����׼�");

    int x = 40;
    int x_offset = 90;
    int y = 65;
    int y_offset = 25;
    //��ʾspeed
    disp_string_rect(x, x_offset, y, y_offset, 5, 24, "speed", BLUE);
    int x2 = 170;
    int x2_offset = 120;
    int y2 = 65;
    int y2_offset = 25;
    //��ʾdirection
		disp_string_rect(x2, x2_offset, y2, y2_offset, 9, 24, "direction", BLUE);
	
    //�򿪱���   
		LCD_BLK_Set();
}

//���������ٶ�+������ʾ
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
