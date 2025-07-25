#ifndef _APP_UI_H_
#define _APP_UI_H_

#include "ti_msp_dl_config.h"

#define screen_center_x (LCD_W/2)//��Ļ����X = ��Ļx/2
#define screen_center_y (LCD_H/2)//��Ļ����Y = ��Ļy/2

#define SPEED_WAVEFORM_REDUCTION_FACTOR 2.3         // ���ٲ���˥������
#define DISTANCE_WAVEFORM_REDUCTION_FACTOR 8.3      // ���ನ��˥������

void ui_home_page(void);                //������ҳ��̬UI
void ui_home_page_select(int mode);     //������ҳѡ���

void ui_speed_page(void);//���ƶ���ҳ��̬UI
void ui_speed_page_value_set(float p, float i, float d, int speed, int target,int quick_update);    //���ƶ���ҳ����ֵ�ı仯
void ui_speed_page_select_box(int mode);//���ƶ���ҳѡ���

void ui_distance_page(void);//���ƶ���ҳ��̬UI
void ui_distance_page_value_set(float p, float i, float d, int distance, int target,int quick_update);//���ƶ���ҳ����ֵ�ı仯

void ui_select_page_show(unsigned char page);//����ѡ��ȷ����ʾ��һ��ҳ��
void ui_parameter_select_box_bold(int mode); //����ѡ���Ӵ֣���ѡ�п�

uint16_t draw_speed_curve(int window_start_x,int window_start_y,int window_w,int window_h,int curve_color,int background_color,short int rawValue);
void ui_speed_curve(void);
void ui_distance_curve(void);
#endif
