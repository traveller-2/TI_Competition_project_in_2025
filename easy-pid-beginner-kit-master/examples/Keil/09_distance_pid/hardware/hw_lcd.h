#ifndef __HW_LCD_H
#define __HW_LCD_H

#include "ti_msp_dl_config.h"

#define LCD_W 320
#define LCD_H 170

#define screen_center_x (LCD_W/2)//��Ļ����X = ��Ļx/2
#define screen_center_y (LCD_H/2)//��Ļ����Y = ��Ļy/2

//-----------------LCD�˿ڶ���----------------

#define LCD_RES_Clr()  DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_RES_PIN)//RES
#define LCD_RES_Set()  DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_RES_PIN)

#define LCD_DC_Clr()   DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_DC_PIN)//DC
#define LCD_DC_Set()   DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_DC_PIN)

#define LCD_CS_Clr()   DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_CS_PIN)//CS
#define LCD_CS_Set()   DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_CS_PIN)

#define LCD_BLK_Clr()  DL_GPIO_clearPins(GPIO_LCD_PORT,GPIO_LCD_PIN_BLK_PIN)//BLK
#define LCD_BLK_Set()  DL_GPIO_setPins(GPIO_LCD_PORT,GPIO_LCD_PIN_BLK_PIN)

void LCD_WR_DATA(unsigned int dat);
void lcd_init(void);
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//ָ�����������ɫ
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);//��ָ��λ�û�һ����
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);//��ָ��λ�û�һ����
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);//��ָ��λ�û�һ������
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color);//��ָ��λ�û�һ��Բ

void LCD_ShowChinese(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ���ִ�
void LCD_ShowChinese12x12(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ����12x12����
void LCD_ShowChinese16x16(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ����16x16����
void LCD_ShowChinese24x24(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ����24x24����
void LCD_ShowChinese32x32(unsigned int x,unsigned int y,unsigned char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ����32x32����

void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾһ���ַ�
void LCD_ShowString(unsigned int x,unsigned int y,const unsigned char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);//��ʾ�ַ���
unsigned int mypow(unsigned char m,unsigned char n);//����
void LCD_ShowIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//��ʾ��������
void LCD_ShowFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//��ʾ��λС������

void LCD_ShowPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[]);//��ʾͼƬ

void LCD_ArcRect(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//��Բ�Ǿ���
void LCD_DrawVerrticalLine(int x, int y1, int y2, unsigned int color);
//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE           	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#endif