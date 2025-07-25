#include "ti_msp_dl_config.h"
#include "mid_debug_led.h"
#include "mid_debug_uart.h"
#include "string.h"
#include "hw_lcd.h"
#include "mid_button.h"
#include "app_key_task.h"

void ui_home_page(void);// ��ҳҳ���ʼ��
void ui_home_page_select(int mode);//����ѡ���

int main(void)
{
		int sys_time = 0;
	
    SYSCFG_DL_init();
		
		//DEBUG���ڳ�ʼ��
		debug_uart_init();
		
		//���������ʼ��
    user_button_init();
		
		//LCD��ʼ��
		lcd_init();
		
		//LCD��ʾUI
		ui_home_page();
	
    while (1) 
		{
			sys_time++;

			if( sys_time % 10 == 0 )//2*10=20ms����һ�ΰ���ɨ��
			{
					//����ɨ��
					flex_button_scan();
			}
			
			if( sys_time % 50 == 0 )//2*50=100msˢ��һ����Ļ
			{
					//��Ļˢ��
					ui_home_page_select( get_app_key_current_mode() );
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
    int str_center_x = (sizey * str_len) / 2; //�ַ���x = �����С*�ַ�������/2
    int rect_center_x = x + (w / 2); //��������x
    int str_center_y = sizey  / 2; //�ַ���y=�����С/2
    int rect_center_y = y + (h / 2); //��������y

    //��ʾ��������    
	  LCD_ArcRect(x, y, x + w, y + h, color);
    //��ʾ�ַ���
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}

/*
����˵������ʾѡ�����������ʾѡ���  x=��ť��ʼX����
        w=��ʾ�İ�ťѡ�����
        y=��ť��ʼY����
        h=��ʾ�İ�ťѡ���߶�
        line_length=ѡ�����߳���
        interval=ѡ�����߼�� ��ťѡ���߿�֮��ľ���
        color=ѡ��������ɫ
*/
void disp_select_box(int x, int w, int y, int h, int line_length, int interval, int color)
{
    //���㰴ťѡ���߿�֮��ľ���+����
    x = x - interval;
    w = w + (interval + interval);
    y = y - interval;
    h = h + (interval + interval);
    //���Ͻ�    
	  LCD_DrawLine(x, y, x + line_length, y, color);
    LCD_DrawLine(x, y, x, y + line_length, color);
    //���Ͻ�    
	  LCD_DrawLine(x + w, y, x + w - line_length, y, color);
    LCD_DrawLine(x + w, y, x + w, y + line_length, color);
    //���½�    
	  LCD_DrawLine(x, y + h, x + line_length, y + h, color);
    LCD_DrawLine(x, y + h, x, y + h - line_length, color);
    //���½�    
	  LCD_DrawLine(x + w, y + h, x + w - line_length, y + h, color);
    LCD_DrawLine(x + w, y + h, x + w, y + h - line_length, color);
}

//�Ҿ�ҳ���ʼ��
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
    int x_offset = 80;
    int y = 65;
    int y_offset = 80;
    //��ʾ��һ����ť������
    disp_string_rect(x, x_offset, y, y_offset, 2, 24, "����", BLUE);
    int x2 = 200;
    int x2_offset = 80;
    int y2 = 65;
    int y2_offset = 80;
    //��ʾ�ڶ�����ť������  
		disp_string_rect(x2, x2_offset, y2, y2_offset, 2, 24, "����", BLUE);
    
    //��ʾѡ���  
		disp_select_box(40,80,65,80,10,5,WHITE);  
	
    //�򿪱���   
		LCD_BLK_Set();
}

//���ݰ���ѡ�������ҳ����ѡ���ѡ���
void ui_home_page_select(int mode)
{
    char select_box_seze = 5;
    switch(mode)
    {
        case 0: //ѡ��PID����ģʽ
            disp_select_box(40,80,65,80,10,select_box_seze,WHITE);
            disp_select_box(200,80,65,80,10,select_box_seze,BLACK); 
            break;
        case 1: //ѡ��PID����ģʽ
            disp_select_box(40,80,65,80,10,select_box_seze,BLACK);
            disp_select_box(200,80,65,80,10,select_box_seze,WHITE);
            break;
    }
}