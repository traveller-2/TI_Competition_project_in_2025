#ifndef _APP_UI_H_
#define _APP_UI_H_

#include "ti_msp_dl_config.h"

#define screen_center_x (LCD_W/2)//屏幕中心X = 屏幕x/2
#define screen_center_y (LCD_H/2)//屏幕中心Y = 屏幕y/2

#define SPEED_WAVEFORM_REDUCTION_FACTOR 2.3         // 定速波形衰减倍数
#define DISTANCE_WAVEFORM_REDUCTION_FACTOR 8.3      // 定距波形衰减倍数

void ui_home_page(void);                //绘制首页静态UI
void ui_home_page_select(int mode);     //绘制首页选择框

void ui_speed_page(void);//绘制定速页静态UI
void ui_speed_page_value_set(float p, float i, float d, int speed, int target,int quick_update);    //绘制定速页参数值的变化
void ui_speed_page_select_box(int mode);//绘制定速页选择框

void ui_distance_page(void);//绘制定距页静态UI
void ui_distance_page_value_set(float p, float i, float d, int distance, int target,int quick_update);//绘制定距页参数值的变化

void ui_select_page_show(unsigned char page);//根据选择确定显示哪一个页面
void ui_parameter_select_box_bold(int mode); //参数选择框加粗，即选中框

uint16_t draw_speed_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue);
void ui_speed_curve(void);
#endif
