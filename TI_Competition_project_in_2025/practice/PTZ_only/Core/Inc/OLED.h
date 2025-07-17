#ifndef __OLED_H__
#define __OLED_H__
 
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"	//����HAL��
/**************************************
    BMPͼƬ����
	ͼƬ��ʽΪ��λ���飬�±�ֱ��ӦͼƬ�Ŀ�͸ߣ�
	BMP_xx[H/8][L];������������ʾһ��ͼ�Σ���Ҫ������һ�£�Ȼ�����OLED_DrawBMP��
**************************************/
extern const uint8_t BMP_Picture[32/8][32];
 
 
/* ����������״̬ */
typedef enum
{
	SET_PIXEL = 0x01,
    RESET_PIXEL = 0x00,
} PixelStatus;

static void OLED_Delay_US(uint32_t us);
	 
/* ���ܺ������� */
//д���ݣ�Ӳ��IICʹ��
void HAL_I2C_WriteByte(uint8_t addr,uint8_t data);
//д����
void WriteCmd(uint8_t IIC_Command);
//д����
void WriteDat(uint8_t IIC_Data);
//��ʼ��OLED
void OLED_Init(void);
//������ʼ������
void OLED_SetPos(unsigned char x, unsigned char y);
//������ɱ�
void OLED_ON(void);
//�رյ�ɱ�
void OLED_OFF(void);
//ˢ�»��������ݵ�GDDRAM
void OLED_RefreshRAM(void);
//������ݻ�����OLED_RAM buffer
void OLED_ClearRAM(void);
//ȫ�����
void OLED_FullyFill(uint8_t fill_Data);
//����
void OLED_FullyClear(void);
//�������д����ʾһ��СȦȦ�õģ������ɾ��
void showO(unsigned char s);
//����������ص�����
PixelStatus OLED_GetPixel(int16_t x, int16_t y);
 
/* ��ʾָ���ַ���ͼƬʱ��Ҫ�ֶ�ˢ�»�������GDDRAM
* function list: OLED_ShowStr\OLED_ShowCN\OLED_Show_MixedCH\OLED_DrawBMP
*/
//��ʾӢ���ַ���
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *ch, unsigned char TextSize);
//��ʾ�����õ�
void OLED_ShowStr1(unsigned char x, unsigned char y, int *ch,unsigned char j, unsigned char TextSize);
//��ʾ�����ַ���
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
//ȫ����ֱ��������
void OLED_VerticalShift(void);
//ȫ��ˮƽ��������
void OLED_HorizontalShift(uint8_t direction);
//ȫ��ͬʱ��ֱ��ˮƽ��������
void OLED_VerticalAndHorizontalShift(uint8_t direction);
//��Ļ����ȡ����ʾ
void OLED_DisplayMode(uint8_t mode);
//��Ļ���ȵ���
void OLED_IntensityControl(uint8_t intensity);
//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          :
// Parameters     : x0,y0 -- ��ʼ������(x0:0~127, y0:0~7); x1,y1 -- ���Խ���(������)������(x1:1~128,y1:1~8)
// Description    : ��ʾBMPλͼ
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
 
 
#endif
