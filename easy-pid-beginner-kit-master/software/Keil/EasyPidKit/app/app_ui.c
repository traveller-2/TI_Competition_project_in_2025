#include "app_ui.h"
#include "string.h"
#include "stdio.h"
#include "hw_lcd.h"
#include "app_sys_mode.h"
#include "app_speed_pid.h"
#include "app_distance_pid.h"
#include "hw_encoder.h"
#include "mid_timer.h"
/*
���ܣ�������ĻX�����ľ��л��Ʋ�ɫ�����Σ������е��ַ���
������
        y=������ʼY��
        str_len=�����ַ�����
        bc=���α���ɫ
        sizey=�ַ����ش�С
        *str=Ҫ��ʾ���ַ���
*/
void disp_x_center(int y, int str_len, uint16_t bc, unsigned char sizey, unsigned char* str)
{
    int str_center_x = (sizey * str_len) / 2;//�ַ�����x=�ַ������ش�С*�ַ����ַ�����/2
    int str_center_y = sizey / 2;//�ַ�����y=�ַ������ش�С/2

    //���Ʊ����Բ�Ǿ���
    LCD_ArcRect(screen_center_x - str_center_x - 10, y, screen_center_x + str_center_x + 10, sizey+y, bc);
    LCD_ShowChinese(screen_center_x - str_center_x,y,str,WHITE,bc,sizey,1);
}

/*
���ܣ����Ʋ�ɫ�����Σ������е��ַ���
������  x=������ʼX��
        w=���ο��
        y=������ʼY��
        h=���θ߶�
        str_len=�ַ�����
        sizey=�ַ����ش�С
        *str=Ҫ��ʾ���ַ���
        color���α���ɫ
��ע��GRAYBLUE ǳ��
      DARKBLUE ����
*/
void disp_string_rect(int x, int w, int y, int h, int str_len, int sizey, unsigned char* str, int color)
{
    int str_center_x = (sizey * str_len) / 2; //�ַ�������x = �ַ������ش�С*�ַ����ַ�����/2
    int rect_center_x = x + (w / 2); //��������x
    int str_center_y = sizey  / 2; //�ַ�����y=�ַ������ش�С/2
    int rect_center_y = y + (h / 2); //��������y

    //���Ʊ�������
    LCD_ArcRect(x, y, x + w, y + h, color);
    //�����ַ���
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}
/*
���ܣ�����ѡ���
������  x=��ʼX���ַ
        w=���Ƶ�ѡ�����ο��
        y=��ʼY���ַ
        h=���Ƶ�ѡ�����θ߶�
        line_length=ѡ�����߳���
        interval=ѡ��� �� ��ѡ����� ֮��ļ������
        color=ѡ������ɫ
*/
void disp_select_box(int x, int w, int y, int h, int line_length, int interval, int color)
{
    //���� ѡ��� �� ��ѡ����� �ľ�����
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

//������ҳ����
void ui_home_page(void)
{
    //�رձ���
    LCD_BLK_Clr();

    //����ȫ�ֱ���
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    //������Դ
    disp_x_center(3,5,BLUE,16,(unsigned char *)"����������");
    //���Ʊ���
    disp_x_center(3+16+3,8,BLUE,16,(unsigned char *)"����PID�����׼�");

    int x = 40;
    int x_offset = 80;
    int y = 65;
    int y_offset = 80;
    //��������һ��PID����
    disp_string_rect(x, x_offset, y, y_offset, 2, 24, "����", BLUE);
    int x2 = 200;
    int x2_offset = 80;
    int y2 = 65;
    int y2_offset = 80;
    //�����������PID����
    disp_string_rect(x2, x2_offset, y2, y2_offset, 2, 24, "����", BLUE);

    //������ҳ��ǰѡ������ ����ѡ���
    switch( get_default_page_flag() )
    {
        case 0:
        disp_select_box(40,80,65,80,10,5,WHITE);
        break;

        case 1:
        disp_select_box(200,80,65,80,10,5,WHITE);
        break;
    }

	LCD_BLK_Set();//�򿪱���
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

typedef struct {
    unsigned int start_x;
    unsigned int start_y;
    unsigned int end_x;
    unsigned int end_y;   
    unsigned int center;
} TXT_OBJECT;

//���ƶ���ҳ����
void ui_speed_page(void)
{
    TXT_OBJECT p={0},i={0},d={0};

    //�رձ���
    LCD_BLK_Clr();

    //����ȫ�ֱ���
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);

    int str_center_x = (24 * 1) / 2;

    //��ʾ��̬�� P I D ����
    LCD_ShowChar(screen_center_x - str_center_x - 84,87,'P',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x,     87,'I',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x + 84,87,'D',WHITE,BLUE,16,1);

    //��ʾ P ������Բ�Ǿ��α���
    p.start_x = screen_center_x - str_center_x - 84 - 30;
    p.start_y = 60+44;
    p.end_x = screen_center_x - str_center_x - 84 + 40;
    p.end_y = 60+44+24;
    LCD_ArcRect(p.start_x, p.start_y, p.end_x, p.end_y, BLUE);

    //��ʾ I ������Բ�Ǿ��α���
    i.start_x = screen_center_x - str_center_x  - 30;
    i.start_y = 60+44;
    i.end_x = screen_center_x - str_center_x  + 40;
    i.end_y = 60+44+24;
    LCD_ArcRect(i.start_x, i.start_y, i.end_x, i.end_y, BLUE);

    //��ʾ D ������Բ�Ǿ��α���
    d.start_x = screen_center_x - str_center_x + 84 - 30;
    d.start_y = 60+44;
    d.end_x = screen_center_x - str_center_x + 84 + 40;
    d.end_y = 60+44+24;
    LCD_ArcRect(d.start_x, d.start_y, d.end_x, d.end_y, BLUE);

    //��ʾ��̬�� Speed:  Target:  ����
    LCD_ShowString(20,170-34,"Speed: ",WHITE,BLUE,24,1);
    LCD_ShowString(320-150,170-34,"Target: ",WHITE,BLUE,24,1);

    LCD_BLK_Set();//�򿪱���
}

/*
���ܣ����ƶ���ҳ����������ʾ
������  p=��ǰPֵ
        i=��ǰIֵ
        d=��ǰDֵ
        speed=��ǰ�ٶ�ֵ
        target=��ǰĿǰֵ
        quick_update=1��������ˢ�� =0�رտ���ˢ��
*/
void ui_speed_page_value_set(float p, float i, float d, int speed, int target,int quick_update)
{
    static float last_p,last_i,last_d;
    static int last_speed,last_target;

    unsigned char show_buff[50] = {0};
    TXT_OBJECT txt_p={0},txt_i={0},txt_d={0};
    int txt_size =0;        //һ�����ַ���������
    int char_width_pixel=8; //16x16��С��Ӣ���ַ�ʵ�ʴ�С��8x16
    int rect_w = 70;        //���εĿ��
    int rect_h = 24;        //���εĸ߶�
    int rect_start_x=34;    //��һ�����ε���ʼX��λ��
    int rect_apart_x=84;    //��������μ�ļ��

    if( quick_update != 1 )//û�п������ٸ���
    {
        last_p = 65535.0;//����һ�������ܵ�ֵ
        last_i = 65535.0;//����һ�������ܵ�ֵ
        last_d = 65535.0;//����һ�������ܵ�ֵ
        last_speed = 65535;//����һ�������ܵ�ֵ
        last_target = 65535;//����һ�������ܵ�ֵ
    }

    if( last_p != p )
    {
       last_p = p;
        sprintf(show_buff, " %.2f ",p);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_p.start_x = rect_start_x + ((rect_w - txt_size) / 2);
        txt_p.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_p.start_x,txt_p.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_i != i )
    {
       last_i = i;
        sprintf(show_buff, " %.2f ",i);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_i.start_x = (rect_start_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_i.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_i.start_x,txt_i.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_d != d )
    {
       last_d = d;
        sprintf(show_buff, " %.2f ",d);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_d.start_x = (rect_start_x + rect_apart_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_d.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_d.start_x,txt_d.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_speed != speed )
    {
       last_speed = speed;
        sprintf(show_buff, "%d ",speed);
        LCD_ShowString(12*6+20,170-34,show_buff,WHITE,BLACK,24,0);
    }
     if( last_target != target )
    {
       last_target = target;
        sprintf(show_buff, "%d ",target);
        LCD_ShowString(320-150 + 12*7,170-34,show_buff,WHITE,BLACK,24,0);
    }
}

//���Ʋ���ѡ���
// mode = 0 ѡ��Pֵ��ʾѡ���
// mode = 1 ѡ��Iֵ��ʾѡ���
// mode = 2 ѡ��Dֵ��ʾѡ���
// mode = 3 ѡ��targetֵ��ʾѡ���
// mode = 4 ȫ������ʾѡ���
void ui_speed_page_select_box(int mode)
{
    char select_box_interval = 3;

    switch(mode)
    {
        case 0: //P
        disp_select_box(34,104-34,104,128-104,10,select_box_interval,WHITE);
        disp_select_box(118,188-118,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(320-150,316-(320-150),170-34,166-(170-34),10,select_box_interval,BLACK);
        break;
        case 1: //I
        disp_select_box(118,188-118,104,128-104,10,select_box_interval,WHITE);
        disp_select_box(202,272-202,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(34,104-34,104,128-104,10,select_box_interval,BLACK);
        break;
        case 2: //D
        disp_select_box(202,272-202,104,128-104,10,select_box_interval,WHITE);
        disp_select_box(320-150,316-(320-150),170-34,166-(170-34),10,select_box_interval,BLACK);
        disp_select_box(118,188-118,104,128-104,10,select_box_interval,BLACK);
        break;        
        case 3: //target
        disp_select_box(320-150,316-(320-150),170-34,166-(170-34),10,select_box_interval,WHITE);
        disp_select_box(34,104-34,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(202,272-202,104,128-104,10,select_box_interval,BLACK);
        break;
        case 4: //all clean
        disp_select_box(34,104-34,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(118,188-118,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(202,272-202,104,128-104,10,select_box_interval,BLACK);
        disp_select_box(320-150,316-(320-150),170-34,166-(170-34),10,select_box_interval,BLACK);
        break;        
    }
}

//���Ʋ���ѡ�п�
// mode = 0 ѡ��Pֵ��ʾѡ�п�
// mode = 1 ѡ��Iֵ��ʾѡ�п�
// mode = 2 ѡ��Dֵ��ʾѡ�п�
// mode = 3 ѡ��targetֵ��ʾѡ�п�
// ..
void ui_parameter_select_box_bold(int mode)
{
    char select_box_seze = 3;
    switch(mode)
    {
        case 0: //P
            LCD_DrawRectangle(34 - select_box_seze+1, 104 - select_box_seze+1, 104 + select_box_seze-1, 128 + select_box_seze-1, WHITE);
        break;
        case 1: //I
            LCD_DrawRectangle(118 - select_box_seze+1, 104 - select_box_seze+1, 188 + select_box_seze-1, 128 + select_box_seze-1, WHITE);
        break;
        case 2: //D
            LCD_DrawRectangle(202 - select_box_seze+1, 104 - select_box_seze+1, 272 + select_box_seze-1, 128 + select_box_seze-1, WHITE);
        break;        
        case 3: //target
            LCD_DrawRectangle(320-150 - select_box_seze+1, 170-34 - select_box_seze+1, 316 + select_box_seze-1, 166 + select_box_seze-1, WHITE);
        break;
       case 4: //P
            LCD_DrawRectangle(34 - select_box_seze+1, 104 - select_box_seze+1, 104 + select_box_seze-1, 128 + select_box_seze-1, BLACK);
        break;
        case 5: //I
            LCD_DrawRectangle(118 - select_box_seze+1, 104 - select_box_seze+1, 188 + select_box_seze-1, 128 + select_box_seze-1, BLACK);
        break;
        case 6: //D
            LCD_DrawRectangle(202 - select_box_seze+1, 104 - select_box_seze+1, 272 + select_box_seze-1, 128 + select_box_seze-1, BLACK);
        break;        
        case 7: //target
            LCD_DrawRectangle(320-150 - select_box_seze+1, 170-34 - select_box_seze+1, 316 + select_box_seze-1, 166 + select_box_seze-1, BLACK);
        break;            
    }
}

//���ƶ���ҳ����
void ui_distance_page(void)
{
    TXT_OBJECT p={0},i={0},d={0};
    //�رձ���
    LCD_BLK_Clr();

    //����ȫ�ֱ���
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);

    int str_center_x = (24 * 1) / 2;

    LCD_ShowChar(screen_center_x - str_center_x - 84,87,'P',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x,     87,'I',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x + 84,87,'D',WHITE,BLUE,16,1);

    p.start_x = screen_center_x - str_center_x - 84 - 30;
    p.start_y = 60+44;
    p.end_x = screen_center_x - str_center_x - 84 + 40;
    p.end_y = 60+44+24;
    LCD_ArcRect(p.start_x, p.start_y, p.end_x, p.end_y, BLUE);

    i.start_x = screen_center_x - str_center_x  - 30;
    i.start_y = 60+44;
    i.end_x = screen_center_x - str_center_x  + 40;
    i.end_y = 60+44+24;
    LCD_ArcRect(i.start_x, i.start_y, i.end_x, i.end_y, BLUE);   

    d.start_x = screen_center_x - str_center_x + 84 - 30;
    d.start_y = 60+44;
    d.end_x = screen_center_x - str_center_x + 84 + 40;
    d.end_y = 60+44+24;
    LCD_ArcRect(d.start_x, d.start_y, d.end_x, d.end_y, BLUE);      

    LCD_ShowString(20,170-34,"Angle: ",WHITE,BLUE,24,1);
    LCD_ShowString(320-150,170-34,"Target: ",WHITE,BLUE,24,1);

    LCD_BLK_Set();//�򿪱���
}

/*
���ܣ����ƶ���ҳ����������ʾ
������  p=��ǰPֵ
        i=��ǰIֵ
        d=��ǰDֵ
        distance=��ǰ�Ƕ�ֵ
        target=��ǰĿǰֵ
        quick_update=1��������ˢ�� =0�رտ���ˢ��
*/
void ui_distance_page_value_set(float p, float i, float d, int distance, int target,int quick_update)
{
    static float last_p,last_i,last_d;
    static int last_distance,last_target;

    unsigned char show_buff[50] = {0};
    TXT_OBJECT txt_p={0},txt_i={0},txt_d={0};
    int txt_size =0;        //һ�����ַ���������  
    int char_width_pixel=8; //16x16��С��Ӣ���ַ�ʵ�ʴ�С��8x16
    int rect_w = 70;        //���εĿ��
    int rect_h = 24;        //���εĸ߶�
    int rect_start_x=34;    //��һ�����ε���ʼX��λ��
    int rect_apart_x=84;    //��������μ�ļ��

    if( quick_update != 1 )//û�п������ٸ���
    {
        last_p = 65535.0;//����һ�������ܵ�ֵ
        last_i = 65535.0;//����һ�������ܵ�ֵ
        last_d = 65535.0;//����һ�������ܵ�ֵ
        last_distance = 65535;//����һ�������ܵ�ֵ
        last_target = 65535;//����һ�������ܵ�ֵ
    }

    if( last_p != p )
    {
       last_p = p;
        sprintf(show_buff, " %.2f ",p);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_p.start_x = rect_start_x + ((rect_w - txt_size) / 2);
        txt_p.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_p.start_x,txt_p.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_i != i )
    {
       last_i = i;
        sprintf(show_buff, " %.2f ",i);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_i.start_x = (rect_start_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_i.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_i.start_x,txt_i.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_d != d )
    {
       last_d = d;
        sprintf(show_buff, " %.2f ",d);
        txt_size = strlen(show_buff) * char_width_pixel;
        txt_d.start_x = (rect_start_x + rect_apart_x + rect_apart_x) + ((rect_w - txt_size) / 2);
        txt_d.start_y = 104 + ((rect_h - 16) / 2 );
        LCD_ShowString(txt_d.start_x,txt_d.start_y,show_buff,WHITE,BLUE,16,0);
    }
    if( last_distance != distance )
    {
        last_distance = distance;
        sprintf(show_buff, "%d ",distance);
        LCD_ShowString(12*6+20,170-34,show_buff,WHITE,BLACK,24,0);
    }
     if( last_target != target )
    {
       last_target = target;   
        sprintf(show_buff, "%d ",target);
        LCD_ShowString(320-150 + 12*7,170-34,show_buff,WHITE,BLACK,24,0);
    }
}

//ѡ��Ҫ��ʾ��һ��ҳ��
// select_flag = 0 ��ʾ����ҳ
// select_flag = 1 ��ʾ����ҳ
// select_flag = 2 ��ʾ��ҳ
void ui_select_page_show(unsigned char select_flag)
{
    if( select_flag == 2 )
    {
        ui_home_page();
    }
    if( select_flag == 0 )
    {
        ui_speed_page();
    }
    if( select_flag == 1 )
    {
        ui_distance_page();
    }
}

/*
*   �������ݣ������ٵ�PID����
*   ����������window_start_x ���ε���ʼX������
              window_start_y ���ε���ʼY������
              window_w ���������������
              window_w �������������߶�
              curve_color �����ߵ���ɫ
              background_color �����������ɫ
              rawValue ����Y������
*   ����ֵ��  ��ǰ���ε�X�������
*/
uint16_t draw_speed_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue)  
{
	uint16_t x=0,y=0,i=0;
	static char firstPoint=1;   //�Ƿ��Ǹոտ�ʼ��,��һ�ν���
	static uint16_t lastX=0,lastY=0;

	//�޷�������С����ֵ
	if( rawValue >= window_h )
	{
    rawValue = window_h;
	}
    if( rawValue <= 0 )
    {
        rawValue = 0;
    }

	//���ڲ��ο� �ײ�Y��� ������ʾ���ݵ�ƫ����
	y = ( window_start_y + window_h ) - rawValue;  	
    
	if(firstPoint)//����ǵ�һ�λ��㣬���������ߣ�ֱ����㼴��
	{
		LCD_DrawPoint(window_start_x,y,curve_color);   
		lastX=window_start_x;
		lastY=y;
		firstPoint=0; 
    return 0;
	}

	//����X��ʱ����
	x=lastX + 1;
	
	if( x < window_w )  //��������Ļ���
	{
			//�����ǰλ�õ�����
			LCD_DrawVerrticalLine(x, window_start_y, window_start_y + window_h, background_color);
			//�ڵ�ǰλ�ø�֮ǰλ��֮������
			LCD_DrawLine(lastX,lastY,x,y,curve_color);
			//��һ�л��ư����߱�ʾX��ˢ�µ�
			LCD_DrawVerrticalLine(x+1, window_start_y, window_start_y + window_h, WHITE);
			//����֮ǰ������Ϊ��ǰ����
			lastX = x;
			lastY = y;
	}
	else  //������Ļ��ȣ��������ӵ�һ���㿪ʼ���ƣ�ʵ�ֶ�̬����Ч��
	{    
			//�����һ����֮ǰ�ĵ�
			LCD_DrawVerrticalLine(window_start_x , window_start_y, window_start_y + window_h, background_color);
			//��ʾ��ǰ�ĵ�
			LCD_DrawPoint(window_start_x, y, curve_color);   
			//����֮ǰ������Ϊ��ǰ����
			lastX = window_start_x;
			lastY = y;
	}
	return x;
}

/*
*   �������ݣ��������PID����
*   ����������window_start_x ���ε���ʼX������
              window_start_y ���ε���ʼY������
              window_w ���������������
              window_w �������������߶�
              curve_color �����ߵ���ɫ
              background_color �����������ɫ
              rawValue ����Y������
*   ����ֵ��  ��ǰ���ε�X�������
*/
uint16_t draw_distance_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue)  
{
	uint16_t x=0,y=0,i=0;
	static char firstPoint=1;   //�Ƿ��Ǹոտ�ʼ��,��һ�ν���
	static uint16_t lastX=0,lastY=0;

	//�޷�������С����ֵ
	if( rawValue >= window_h )
	{
			rawValue = window_h;
	}
	if( rawValue <= 0 )
	{
			rawValue = 0;
	}
    
	//���ڲ��ο� �ײ�Y��� ������ʾ���ݵ�ƫ����
	y = ( window_start_y + window_h ) - rawValue;  	
    
	if(firstPoint)//����ǵ�һ�λ��㣬���������ߣ�ֱ����㼴��
	{
		LCD_DrawPoint(window_start_x,y,curve_color);   
		lastX=window_start_x;
		lastY=y;
		firstPoint=0; 
		return 0;
	}

	x=lastX+1;
	
	if( x < window_w )  //��������Ļ���
	{
			LCD_DrawVerrticalLine(x, window_start_y, window_start_y + window_h, background_color);
			
			LCD_DrawLine(lastX,lastY,x,y,curve_color);
			//�����һ���ط�����ʾ����
			LCD_DrawVerrticalLine(x+1, window_start_y, window_start_y + window_h, WHITE);
			lastX = x;
			lastY = y;
	}
	else  //������Ļ��ȣ��������ӵ�һ���㿪ʼ���ƣ�ʵ�ֶ�̬����Ч��
	{        
			LCD_DrawVerrticalLine(window_start_x , window_start_y, window_start_y + window_h, background_color);
			LCD_DrawPoint(window_start_x, y, curve_color);   
			lastX = window_start_x;
			lastY = y;
	}
	return x;
}

//UI��ʾ����ҳ��PID���κͲ���
void ui_speed_curve(void)
{
    disable_task_interrupt(); //��ֹ�������

    ui_speed_page_value_set(get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd, \
                            get_encoder_count(), get_speed_pid()->target, 1);
    int curve_x = 0;
    //��+ SPEED_ENCODER_VALUE_MAX�� ����������ֵ�Ŵ�ȥ������
    //��/ SPEED_WAVEFORM_REDUCTION_FACTOR ��Ϊ��ĻС�Ų��±��������ֵ����Сֵ�����������˥����ֵ
    //���Ƶ�ǰ��������ֵ����
    curve_x = draw_speed_curve(0,0,319,88,GREEN,BLACK,( get_encoder_count() + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR );  
    
    //����Ŀ���ٶȵĲ��ε�
    LCD_DrawPoint(curve_x, 88 - ((get_speed_pid_target() + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);

    enable_task_interrupt(); //�����������
}
//UI��ʾ����ҳ��PID���κͲ���
void ui_distance_curve(void)
{
    int current_angle = 0;

    disable_task_interrupt(); //��ֹ�������
    
    current_angle = get_temp_encoder() * DEGREES_PER_PULSE;

    ui_distance_page_value_set(get_distance_pid()->kp, get_distance_pid()->ki, get_distance_pid()->kd,
                            current_angle, get_distance_pid()->target, 1);

    int curve_x = 0;
    //���Ƶ�ǰ��������ֵ����
    curve_x = draw_distance_curve(0,0,319,88,GREEN,BLACK,( current_angle + DISTANCE_ENCODER_VALUE_MAX ) / DISTANCE_WAVEFORM_REDUCTION_FACTOR );  
    //����Ŀ���ٶȵĲ��ε�
    LCD_DrawPoint(curve_x, 88 - ((get_distance_pid_target() + DISTANCE_ENCODER_VALUE_MAX ) / DISTANCE_WAVEFORM_REDUCTION_FACTOR), RED);

    enable_task_interrupt(); //�����������
}