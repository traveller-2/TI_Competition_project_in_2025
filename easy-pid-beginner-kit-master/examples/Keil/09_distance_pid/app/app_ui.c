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
功能：基于屏幕X轴中心居中绘制彩色填充矩形，带居中的字符串
参数：
        y=矩形起始Y轴
        str_len=中文字符个数
        bc=矩形背景色
        sizey=字符像素大小
        *str=要显示的字符串
*/
void disp_x_center(int y, int str_len, uint16_t bc, unsigned char sizey, unsigned char* str)
{
    int str_center_x = (sizey * str_len) / 2;//字符中心x=字符串像素大小*字符串字符个数/2
    int str_center_y = sizey / 2;//字符中心y=字符串像素大小/2

    //绘制标题的圆角矩形
    LCD_ArcRect(screen_center_x - str_center_x - 10, y, screen_center_x + str_center_x + 10, sizey+y, bc);
    LCD_ShowChinese(screen_center_x - str_center_x,y,str,WHITE,bc,sizey,1);
}

/*
功能：绘制彩色填充矩形，带居中的字符串
参数：  x=矩形起始X轴
        w=矩形宽度
        y=矩形起始Y轴
        h=矩形高度
        str_len=字符个数
        sizey=字符像素大小
        *str=要显示的字符串
        color矩形背景色
备注：GRAYBLUE 浅蓝
      DARKBLUE 深蓝
*/
void disp_string_rect(int x, int w, int y, int h, int str_len, int sizey, unsigned char* str, int color)
{
    int str_center_x = (sizey * str_len) / 2; //字符串中心x = 字符串像素大小*字符串字符个数/2
    int rect_center_x = x + (w / 2); //矩形中心x
    int str_center_y = sizey  / 2; //字符中心y=字符串像素大小/2
    int rect_center_y = y + (h / 2); //矩形中心y

    //绘制背景矩形
    LCD_ArcRect(x, y, x + w, y + h, color);
    //绘制字符串
    LCD_ShowChinese(rect_center_x - str_center_x, rect_center_y - str_center_y,str,WHITE,color,sizey,1);
}
/*
功能：绘制选择框
参数：  x=起始X轴地址
        w=绘制的选择框矩形宽度
        y=起始Y轴地址
        h=绘制的选择框矩形高度
        line_length=选择框的线长度
        interval=选择框 与 被选择矩形 之间的间隔像素
        color=选择框的颜色
*/
void disp_select_box(int x, int w, int y, int h, int line_length, int interval, int color)
{
    //计算 选择框 与 被选择矩形 的距离间隔
    x = x - interval;
    w = w + (interval + interval);
    y = y - interval;
    h = h + (interval + interval);
    //左上角
    LCD_DrawLine(x, y, x + line_length, y, color);
    LCD_DrawLine(x, y, x, y + line_length, color);
    //右上角
    LCD_DrawLine(x + w, y, x + w - line_length, y, color);
    LCD_DrawLine(x + w, y, x + w, y + line_length, color);
    //左下角
    LCD_DrawLine(x, y + h, x + line_length, y + h, color);
    LCD_DrawLine(x, y + h, x, y + h - line_length, color);
    //右下角
    LCD_DrawLine(x + w, y + h, x + w - line_length, y + h, color);
    LCD_DrawLine(x + w, y + h, x + w, y + h - line_length, color);
}

//绘制首页界面
void ui_home_page(void)
{
    //关闭背光
    LCD_BLK_Clr();

    //绘制全局背景
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
    //绘制来源
    disp_x_center(3,5,BLUE,16,(unsigned char *)"立创开发板");
    //绘制标题
    disp_x_center(3+16+3,8,BLUE,16,(unsigned char *)"简易PID入门套件");

    int x = 40;
    int x_offset = 80;
    int y = 65;
    int y_offset = 80;
    //绘制任务一：PID定速
    disp_string_rect(x, x_offset, y, y_offset, 2, 24, "定速", BLUE);
    int x2 = 200;
    int x2_offset = 80;
    int y2 = 65;
    int y2_offset = 80;
    //绘制任务二：PID定距
    disp_string_rect(x2, x2_offset, y2, y2_offset, 2, 24, "定距", BLUE);

    //根据首页当前选择内容 绘制选择框
    switch( get_default_page_flag() )
    {
        case 0:
        disp_select_box(40,80,65,80,10,5,WHITE);
        break;

        case 1:
        disp_select_box(200,80,65,80,10,5,WHITE);
        break;
    }

	LCD_BLK_Set();//打开背光
}

//根据按键选择绘制首页两个选项的选择框
void ui_home_page_select(int mode)
{
    char select_box_seze = 5;
    switch(mode)
    {
        case 0: //选择PID定速模式
            disp_select_box(40,80,65,80,10,select_box_seze,WHITE);
            disp_select_box(200,80,65,80,10,select_box_seze,BLACK); 
            break;
        case 1: //选择PID定距模式
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

//绘制定速页界面
void ui_speed_page(void)
{
    TXT_OBJECT p={0},i={0},d={0};

    //关闭背光
    LCD_BLK_Clr();

    //绘制全局背景
    LCD_Fill(0,0,LCD_W,LCD_H,BLACK);

    int str_center_x = (24 * 1) / 2;

    //显示静态的 P I D 标题
    LCD_ShowChar(screen_center_x - str_center_x - 84,87,'P',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x,     87,'I',WHITE,BLUE,16,1);
    LCD_ShowChar(screen_center_x - str_center_x + 84,87,'D',WHITE,BLUE,16,1);

    //显示 P 参数的圆角矩形背景
    p.start_x = screen_center_x - str_center_x - 84 - 30;
    p.start_y = 60+44;
    p.end_x = screen_center_x - str_center_x - 84 + 40;
    p.end_y = 60+44+24;
    LCD_ArcRect(p.start_x, p.start_y, p.end_x, p.end_y, BLUE);

    //显示 I 参数的圆角矩形背景
    i.start_x = screen_center_x - str_center_x  - 30;
    i.start_y = 60+44;
    i.end_x = screen_center_x - str_center_x  + 40;
    i.end_y = 60+44+24;
    LCD_ArcRect(i.start_x, i.start_y, i.end_x, i.end_y, BLUE);

    //显示 D 参数的圆角矩形背景
    d.start_x = screen_center_x - str_center_x + 84 - 30;
    d.start_y = 60+44;
    d.end_x = screen_center_x - str_center_x + 84 + 40;
    d.end_y = 60+44+24;
    LCD_ArcRect(d.start_x, d.start_y, d.end_x, d.end_y, BLUE);

    //显示静态的 Speed:  Target:  标题
    LCD_ShowString(20,170-34,"Speed: ",WHITE,BLUE,24,1);
    LCD_ShowString(320-150,170-34,"Target: ",WHITE,BLUE,24,1);

    LCD_BLK_Set();//打开背光
}

/*
功能：绘制定速页参数更新显示
参数：  p=当前P值
        i=当前I值
        d=当前D值
        speed=当前速度值
        target=当前目前值
        quick_update=1开启快速刷新 =0关闭快速刷新
*/
void ui_speed_page_value_set(float p, float i, float d, int speed, int target,int quick_update)
{
    static float last_p,last_i,last_d;
    static int last_speed,last_target;

    unsigned char show_buff[50] = {0};
    TXT_OBJECT txt_p={0},txt_i={0},txt_d={0};
    int txt_size =0;        //一整个字符串的像素
    int char_width_pixel=8; //16x16大小的英文字符实际大小是8x16
    int rect_w = 70;        //矩形的宽度
    int rect_h = 24;        //矩形的高度
    int rect_start_x=34;    //第一个矩形的起始X轴位置
    int rect_apart_x=84;    //矩形与矩形间的间隔

    if( quick_update != 1 )//没有开启快速更新
    {
        last_p = 65535.0;//设置一个不可能的值
        last_i = 65535.0;//设置一个不可能的值
        last_d = 65535.0;//设置一个不可能的值
        last_speed = 65535;//设置一个不可能的值
        last_target = 65535;//设置一个不可能的值
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

//绘制参数选择框
// mode = 0 选择P值显示选择框
// mode = 1 选择I值显示选择框
// mode = 2 选择D值显示选择框
// mode = 3 选择target值显示选择框
// mode = 4 全部不显示选择框
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

//绘制参数选中框
// mode = 0 选择P值显示选中框
// mode = 1 选择I值显示选中框
// mode = 2 选择D值显示选中框
// mode = 3 选择target值显示选中框
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

//绘制定距页内容
void ui_distance_page(void)
{
    TXT_OBJECT p={0},i={0},d={0};
    //关闭背光
    LCD_BLK_Clr();

    //绘制全局背景
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

    LCD_BLK_Set();//打开背光
}

/*
功能：绘制定距页参数更新显示
参数：  p=当前P值
        i=当前I值
        d=当前D值
        distance=当前角度值
        target=当前目前值
        quick_update=1开启快速刷新 =0关闭快速刷新
*/
void ui_distance_page_value_set(float p, float i, float d, int distance, int target,int quick_update)
{
    static float last_p,last_i,last_d;
    static int last_distance,last_target;

    unsigned char show_buff[50] = {0};
    TXT_OBJECT txt_p={0},txt_i={0},txt_d={0};
    int txt_size =0;        //一整个字符串的像素  
    int char_width_pixel=8; //16x16大小的英文字符实际大小是8x16
    int rect_w = 70;        //矩形的宽度
    int rect_h = 24;        //矩形的高度
    int rect_start_x=34;    //第一个矩形的起始X轴位置
    int rect_apart_x=84;    //矩形与矩形间的间隔

    if( quick_update != 1 )//没有开启快速更新
    {
        last_p = 65535.0;//设置一个不可能的值
        last_i = 65535.0;//设置一个不可能的值
        last_d = 65535.0;//设置一个不可能的值
        last_distance = 65535;//设置一个不可能的值
        last_target = 65535;//设置一个不可能的值
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

//选择要显示哪一个页面
// select_flag = 0 显示定速页
// select_flag = 1 显示定距页
// select_flag = 2 显示首页
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
*   函数内容：画定速的PID波形
*   函数参数：window_start_x 波形的起始X轴坐标
              window_start_y 波形的起始Y轴坐标
              window_w 波形组件的整体宽度
              window_w 波形组件的整体高度
              curve_color 波形线的颜色
              background_color 波形组件背景色
              rawValue 波形Y轴数据
*   返回值：  当前波形的X轴坐标点
*/
uint16_t draw_speed_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue)  
{
	uint16_t x=0,y=0,i=0;
	static char firstPoint=1;   //是否是刚刚开始画,第一次进入
	static uint16_t lastX=0,lastY=0;

	//限幅最大和最小输入值
	if( rawValue >= window_h )
	{
    rawValue = window_h;
	}
    if( rawValue <= 0 )
    {
        rawValue = 0;
    }

	//基于波形框 底部Y轴点 计算显示数据的偏移量
	y = ( window_start_y + window_h ) - rawValue;  	
    
	if(firstPoint)//如果是第一次画点，则无需连线，直接描点即可
	{
		LCD_DrawPoint(window_start_x,y,curve_color);   
		lastX=window_start_x;
		lastY=y;
		firstPoint=0; 
    return 0;
	}

	//更新X轴时间线
	x=lastX + 1;
	
	if( x < window_w )  //不超过屏幕宽度
	{
			//清除当前位置的内容
			LCD_DrawVerrticalLine(x, window_start_y, window_start_y + window_h, background_color);
			//在当前位置跟之前位置之间连线
			LCD_DrawLine(lastX,lastY,x,y,curve_color);
			//下一列绘制白竖线表示X轴刷新点
			LCD_DrawVerrticalLine(x+1, window_start_y, window_start_y + window_h, WHITE);
			//更新之前的坐标为当前坐标
			lastX = x;
			lastY = y;
	}
	else  //超出屏幕宽度，清屏，从第一个点开始绘制，实现动态更新效果
	{    
			//清除第一列中之前的点
			LCD_DrawVerrticalLine(window_start_x , window_start_y, window_start_y + window_h, background_color);
			//显示当前的点
			LCD_DrawPoint(window_start_x, y, curve_color);   
			//更新之前的坐标为当前坐标
			lastX = window_start_x;
			lastY = y;
	}
	return x;
}

/*
*   函数内容：画定距的PID波形
*   函数参数：window_start_x 波形的起始X轴坐标
              window_start_y 波形的起始Y轴坐标
              window_w 波形组件的整体宽度
              window_w 波形组件的整体高度
              curve_color 波形线的颜色
              background_color 波形组件背景色
              rawValue 波形Y轴数据
*   返回值：  当前波形的X轴坐标点
*/
uint16_t draw_distance_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue)  
{
	uint16_t x=0,y=0,i=0;
	static char firstPoint=1;   //是否是刚刚开始画,第一次进入
	static uint16_t lastX=0,lastY=0;

	//限幅最大和最小输入值
	if( rawValue >= window_h )
	{
			rawValue = window_h;
	}
	if( rawValue <= 0 )
	{
			rawValue = 0;
	}
    
	//基于波形框 底部Y轴点 计算显示数据的偏移量
	y = ( window_start_y + window_h ) - rawValue;  	
    
	if(firstPoint)//如果是第一次画点，则无需连线，直接描点即可
	{
		LCD_DrawPoint(window_start_x,y,curve_color);   
		lastX=window_start_x;
		lastY=y;
		firstPoint=0; 
		return 0;
	}

	x=lastX+1;
	
	if( x < window_w )  //不超过屏幕宽度
	{
			LCD_DrawVerrticalLine(x, window_start_y, window_start_y + window_h, background_color);
			
			LCD_DrawLine(lastX,lastY,x,y,curve_color);
			//清除下一个地方的显示内容
			LCD_DrawVerrticalLine(x+1, window_start_y, window_start_y + window_h, WHITE);
			lastX = x;
			lastY = y;
	}
	else  //超出屏幕宽度，清屏，从第一个点开始绘制，实现动态更新效果
	{        
			LCD_DrawVerrticalLine(window_start_x , window_start_y, window_start_y + window_h, background_color);
			LCD_DrawPoint(window_start_x, y, curve_color);   
			lastX = window_start_x;
			lastY = y;
	}
	return x;
}

//UI显示定速页的PID波形和参数
void ui_speed_curve(void)
{
    disable_task_interrupt(); //禁止任务调度

    ui_speed_page_value_set(get_speed_pid()->kp, get_speed_pid()->ki, get_speed_pid()->kd, \
                            get_encoder_count(), get_speed_pid()->target, 1);
    int curve_x = 0;
    //“+ SPEED_ENCODER_VALUE_MAX” 将编码器数值放大，去除负数
    //“/ SPEED_WAVEFORM_REDUCTION_FACTOR 因为屏幕小放不下编码器最大值和最小值，因此做除法衰减数值
    //绘制当前编码器数值曲线
    curve_x = draw_speed_curve(0,0,319,88,GREEN,BLACK,( get_encoder_count() + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR );  
    
    //绘制目标速度的波形点
    LCD_DrawPoint(curve_x, 88 - ((get_speed_pid_target() + SPEED_ENCODER_VALUE_MAX ) / SPEED_WAVEFORM_REDUCTION_FACTOR), RED);

    enable_task_interrupt(); //开启任务调度
}
//UI显示定距页的PID波形和参数
void ui_distance_curve(void)
{
    int current_angle = 0;

    disable_task_interrupt(); //禁止任务调度
    
    current_angle = get_temp_encoder() * DEGREES_PER_PULSE;

    ui_distance_page_value_set(get_distance_pid()->kp, get_distance_pid()->ki, get_distance_pid()->kd,
                            current_angle, get_distance_pid()->target, 1);

    int curve_x = 0;
    //绘制当前编码器数值曲线
    curve_x = draw_distance_curve(0,0,319,88,GREEN,BLACK,( current_angle + DISTANCE_ENCODER_VALUE_MAX ) / DISTANCE_WAVEFORM_REDUCTION_FACTOR );  
    //绘制目标速度的波形点
    LCD_DrawPoint(curve_x, 88 - ((get_distance_pid_target() + DISTANCE_ENCODER_VALUE_MAX ) / DISTANCE_WAVEFORM_REDUCTION_FACTOR), RED);

    enable_task_interrupt(); //开启任务调度
}