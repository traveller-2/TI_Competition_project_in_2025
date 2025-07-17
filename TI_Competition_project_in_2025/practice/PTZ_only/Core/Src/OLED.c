/***********************************
	�������ļ�������HAL��汾
***********************************/
#include "stm32f4xx_hal.h"	//����HAL��
#include "CharLib_CN.h"	 //�ֿ��ļ�
#include "OLED.h"		//����
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "motor_monitor.h"
#include "imu901.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_i2c.h"
#include "bsp_vl53l0x.h"
#include "hc_sr04.h"
#include "servo.h"
#include "bt_comm.h"

/* ���ƺ� *///����Ҫ�õ������Բ���֪��ʲô����
#define LEFT 			0x27
#define RIGHT 		0x26
#define UP 				0X29
#define DOWM 			0x2A
#define ON				0xA7
#define OFF				0xA6
 
 
/* IIC�ӿ�ѡ�� */
#define IICx hi2c2

extern I2C_HandleTypeDef hi2c2;	//HAL��ʹ�ã�ָ��Ӳ��IIC�ӿ�
 
// �����ļ������궨������������֮��
static void OLED_Delay_US(uint32_t us)
{
    uint32_t count = (168 * us) / 5;  // ����ѭ��������168MHz����ѭ��Լ5ָ�
    while (count--) __NOP();
}

 
//oled��ʾ�ߴ�
uint16_t const displayWidth                = 130;//������Ӧ����128��������ʾ��ʱ���аױߣ����Ըĳ�130
uint16_t const displayHeight               = 64;
 
/*  OLED�Դ�
[0]0 1 2 3 ... 127
[1]0 1 2 3 ... 127
[2]0 1 2 3 ... 127
[3]0 1 2 3 ... 127
[4]0 1 2 3 ... 127
[5]0 1 2 3 ... 127
[6]0 1 2 3 ... 127
[7]0 1 2 3 ... 127 */
 
static uint8_t OLED_RAM[8][130];//����GDDRAM��������ûɶ�ã�


void MotorIMU_Monitor_Task(void *argument)
{
    OLED_ShowStr(0, 0, (uint8_t *)"Divices Monitor", 1);

    char buffer[20];

    while (1)
    {
        // ��ʾ RPM

				// ��2�У�RPM1 + EER1
				sprintf(buffer, "RPM1:%4d", rpm[0] );
				OLED_ShowStr(0, 2, (uint8_t *)buffer, 1);

				// ��3�У�RPM2 + EER2
				sprintf(buffer, "RPM2:%4d", rpm[1] );
				OLED_ShowStr(0, 3, (uint8_t *)buffer, 1);



				sprintf(buffer, "TOF:%4dmm H:%4dcm", vl53l0x_distance_mm, hcsr04_distance_mm);
				OLED_ShowStr(0, 4, (uint8_t *)buffer, 1);

        // ��̬��
        imu901_attitude_t att = imu901_get_attitude();

        // ��5�У�P + S0
        sprintf(buffer, "P:%6.1f S0:%4d", att.pitch , (servo_angle[0]-500)* 180 / 2000);
        OLED_ShowStr(0, 5, (uint8_t *)buffer, 1);

        // ��6�У�R + S1
        sprintf(buffer, "R:%6.1f S1:%4d", att.roll - 7.7f , (servo_angle[1]-500)* 180 / 2000);
        OLED_ShowStr(0, 6, (uint8_t *)buffer, 1);

        // ��7�У�Y
        sprintf(buffer, "Y:%6.1f", att.yaw);
        OLED_ShowStr(0, 7, (uint8_t *)buffer, 1);

        osDelay(50);  // ����Ϊ 200~300ms
    }
}


//void Start_Oled()
//{
//			for(int i = 0 ; i < 100 ; i++)
//		{
//			
//			//�±������ǽ�������Ϊ�ַ������룬ע���ַ�����С
//			char buffer[20];
//			sprintf(buffer,"i = %d ",i);
//			OLED_ShowStr(0, 0, (uint8_t*)buffer,1);
//			//
//			
//			osDelay(2000);
//		}
//}



 
/***************************************************
    I2C���ߴ������ݺ�����
                addr  :    Ҫд��ĵ�ַ��OLED�ĵ�ַһ��Ϊ0x40;ָ���ַΪ0x00��
                data  :    Ҫд�������
***************************************************/
void HAL_I2C_WriteByte(uint8_t addr, uint8_t data)
{
    uint8_t TxData[2] = {addr, data};
    HAL_I2C_Master_Transmit(&IICx, 0x78, TxData, 2, 10);
    //OLED_Delay_US(5);  // ��ֹI2Cд�����
}

 
/**************************************************************
	 Prototype      : void WriteCmd(uint8_t IIC_Command)
	 Parameters     : IIC_Command
	 return					: none
	 Description    : д���ͨ��HAL_I2C_WriteByte�е�HAL_I2C_Master_Transmit
                      ��0x00д�����
***************************************************************/
void WriteCmd(uint8_t IIC_Command)
{
    HAL_I2C_WriteByte(0x00, IIC_Command);
    OLED_Delay_US(5);  // ��ʱ��ֹд�������
}

 
/**************************************************************
	 Prototype      : void WriteDat(uint8_t IIC_Data)
	 Parameters     : IIC_Data
	 return					: none
	 Description    : д���ݣ�ͨ��HAL_I2C_WriteByte�е�HAL_I2C_Master_Transmit
                      ��0x40д�����ݣ�
***************************************************************/
void WriteDat(uint8_t IIC_Data)
{
    HAL_I2C_WriteByte(0x40, IIC_Data);
    //OLED_Delay_US(5);  // ��ʱ��ֹд���ݹ���
}
 
/**************************************************************
	 Prototype      : void OLED_Init(void)
	 Parameters     : none
	 return					: none
	 Description    : ��ʼ��OLEDģ�飨���ѷ��֣���ʵ����һЩ��0x00д������
***************************************************************/
void OLED_Init(void)
{
	HAL_Delay(500); //HAL��ʱ����
 
	WriteCmd(0xAE); //����ʾ
	WriteCmd(0x20);	//�����ڴ�Ѱַģʽ
 
	WriteCmd(0x10);	//00��ˮƽѰַģʽ;01����ֱѰַģʽ;10��ҳ��Ѱַģʽ(����);11����Ч
	WriteCmd(0xb0);	//Ϊҳ��Ѱַģʽ����ҳ�濪ʼ��ַ��0-7
	WriteCmd(0x00); //---���õ��е�ַ
	WriteCmd(0x10); //---���ø��е�ַ
 
	WriteCmd(0xc8);	//����COM���ɨ�跽��
	WriteCmd(0x40); //--������ʼ�е�ַ
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--���ö�����ӳ��0��127
	WriteCmd(0xa6); //--����������ʾ
	WriteCmd(0xa8); //--���ø��ñ�(1 ~ 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,�����ѭRAM����;0xa5,Output����RAM����
	WriteCmd(0xd3); //-������ʾ����
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--������ʾʱ�ӷ�Ƶ/����Ƶ��
	WriteCmd(0xf0); //--���÷���
	WriteCmd(0xd9); //--����pre-chargeʱ��
	WriteCmd(0x22); //
	WriteCmd(0xda); //--����com��ͷ��Ӳ������
	WriteCmd(0x12);
	WriteCmd(0xdb); //--����vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--����DC-DC
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--��oled���
 
	OLED_FullyClear();//����
}
 
void OLED_SetPos(unsigned char x, unsigned char y)
{
    WriteCmd(0xb0 + y);
    OLED_Delay_US(2);

    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_Delay_US(2);

    WriteCmd((x & 0x0f) | 0x01);
    OLED_Delay_US(2);
}
 
/**************************************************************
	 Prototype      : void OLED_ON(void)
	 Parameters     : none
	 return					: none
	 Description    : ��OLED�������л��ѣ���ʼ����ʱ���Ѿ�����ˣ�����Բ����ǣ�
***************************************************************/
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}
 
/**************************************************************
	 Prototype      : void OLED_OFF(void)
	 Parameters     : none
	 return					: none
	 Description    : ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
***************************************************************/
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}
 
/**************************************************************
	 Prototype      : void OLED_RefreshRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : ȫ�����
***************************************************************/
void OLED_RefreshRAM(void)
{
    for (uint16_t m = 0; m < displayHeight / 8; m++)
    {
        WriteCmd(0xb0 + m);
        WriteCmd(0x00);
        WriteCmd(0x10);
        //OLED_Delay_US(10);  // �������л���ʱ

        for (uint16_t n = 0; n < displayWidth; n++)
        {
            WriteDat(OLED_RAM[m][n]);
        }
    }
}


/**************************************************************
	 Prototype      : void OLED_ClearRAM(void)
	 Parameters     : none
	 return					: none
	 Description    : ������ݻ�������ǰ��˵�ˣ������������ò�����
***************************************************************/
void OLED_ClearRAM(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = 0x00;
		}
	}
}
 
 
/**************************************************************
	 Prototype      : void OLED_Fill(uint8_t fill_Data)
	 Parameters     : fill_Data ����1�ֽ�����
	 return					: none
	 Description    : ȫ����� 0x00~0xff
***************************************************************/
void OLED_FullyFill(unsigned char fill_Data)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = fill_Data;
		}
	}
 
	OLED_RefreshRAM();
}
 
/**************************************************************
	 Prototype      : void OLED_FullyClear(void)
	 Parameters     : none
	 return					: none
	 Description    : ȫ�����
***************************************************************/
void OLED_FullyClear(void)
{
		OLED_FullyFill(RESET_PIXEL);
}
 
/**************************************************************
	Prototype      :  void OLED_GetPixel(int16_t x, int16_t y)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63);
	return				 :  PixelStatus ���ص�״̬ 	SET_PIXEL = 1, RESET_PIXEL = 0
	Description    : 	����������ص����ݣ�����0.96�����Ļ��˵��ûɶ�ã�
***************************************************************/
PixelStatus OLED_GetPixel(int16_t x, int16_t y)
{
	 if(OLED_RAM[y/8][x] >> (y%8) & 0x01)
		 return SET_PIXEL;
 
	return	RESET_PIXEL;
}
 
/**************************************************************
	Prototype      : void OLED_ShowStr1(unsigned char x, unsigned char y, int *ch,
                     unsigned char s, unsigned char TextSize)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63);
						*ch -- Ҫ��ʾ������;
                        s----- ���ֵ�λ��
					    TextSize -- �ַ���С(1:6*8 ; 2:8*16)
	return	       :    none
	Description    : 	��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ�����Լ�д�ģ�
***************************************************************/
void OLED_ShowStr1(unsigned char x, unsigned char y, int *ch, unsigned char s, unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j=0,k=0;
	switch(TextSize)
	{
		case 1:
		{
			while(s--)
			{
				c = (ch[j] +16);
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				 x += 6;
				 j++;
			}
		}break;
		case 2:
		{
			while(s--)
			{
				c = (ch[j] +16);
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					 WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}
 
 
/**************************************************************
	 Prototype      : void OLED_ShowCN(int16_t x, int16_t y, uint8_t* n)
	 Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7);
					  N[]:������codetab.h�е����������ǵڼ��У�
	 return		    : none
	 Description    : ��ʾcodetab.h�еĺ���,16*16����
***************************************************************/
 
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	 for(wm = 0;wm < 16;wm++)
	{
		 WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}
/**************************************************************
	 Prototype      :	void OLED_FullyToggle(void)
	 Parameters     : none
	 return				  : none
	 Description    : ����������ȡ����ˢ�µ�GDDRAM��������˼�ɣ�
***************************************************************/
void OLED_FullyToggle(void)
{
	for(uint16_t m = 0; m < displayHeight/8; m++)
	{
		for(uint16_t n = 0; n < displayWidth; n++)
		{
				OLED_RAM[m][n] = ~OLED_RAM[m][n];
		}
	}
	OLED_RefreshRAM();
}
 
/****************************************************************
	ȫ����ֱƫ��,0->63����
	����ֱ����,��Χ0-63
	����ֱ����,��Χ63-0
****************************************************************/
void OLED_VerticalShift(void)
{
	for(uint8_t i = 0; i < displayHeight; i++)
		{
			WriteCmd(0xd3);//������ʾƫ�ƣ�0->63����
			WriteCmd(i);//ƫ����
			HAL_Delay(40);//��ʱʱ��
		}
}
 
/****************************************************************
	��Ļ����ˮƽȫ����������
	��		LEFT	0x27
	��		RIGHT	0x26
****************************************************************/
void OLED_HorizontalShift(uint8_t direction)
 
{
	WriteCmd(direction);//���ù�������
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0x00);//���ÿ�ʼҳ��ַ
	WriteCmd(0x05);//����ÿ����������֮���ʱ������֡Ƶ
	WriteCmd(0x07);//���ý���ҳ��ַ
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0xff);//�����ֽ����ã�Ĭ��Ϊ0xff
	WriteCmd(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}
 
/****************************************************************
	��Ļ���ݴ�ֱˮƽȫ����������
	��		UP		0x29
	��		DOWN	0x2A
****************************************************************/
void OLED_VerticalAndHorizontalShift(uint8_t direction)
{
	WriteCmd(direction);//���ù�������
	WriteCmd(0x00);//�����ֽ����ã�Ĭ��Ϊ0x00
	WriteCmd(0x00);//���ÿ�ʼҳ��ַ
	WriteCmd(0x05);//����ÿ����������֮���ʱ������֡Ƶ
	WriteCmd(0x07);//���ý���ҳ��ַ
	WriteCmd(0x01);//��ֱ����ƫ����
 
	WriteCmd(0x2f);//��������-0x2f�����ù���-0x2e��������Ҫ��д����
}
 
/****************************************************************
	��Ļ����ȡ����ʾ
	��	ON	0xA7
	��	OFF	0xA6	Ĭ�ϴ�ģʽ���������ص���
****************************************************************/
void OLED_DisplayMode(uint8_t mode)
{
	WriteCmd(mode);
}
 
/****************************************************************
	��Ļ���ȵ���
	intensity	0-255
	Ĭ��Ϊ0x7f
****************************************************************/
void OLED_IntensityControl(uint8_t intensity)
{
	WriteCmd(0x81);
	WriteCmd(intensity);
}
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])//X0��ͼ�����ʼλ�ã�y0��ͼ�����ʼ�У�X1��ͼ��Ŀ�ȣ�Y1��ͼ�����ʾ�߶�1~8��
{
	unsigned int j=0;
	unsigned char x,y;
 
  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}
/************************************************************
    Prototype      : void OLED_ShowStr1(unsigned char x, unsigned char y, int *ch,
                     unsigned char s, unsigned char TextSize)
	Parameters     : 	x,y -- ��ʼ������(x:0~127, y:0~63);
						*ch -- Ҫ��ʾ��ASCII�ַ�,����ֱ�Ӵ����ַ���;
					    TextSize -- �ַ���С(1:6*8 ; 2:8*16)
	return	       :    none
	Description    : 	��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
 ***********************************************************/
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char *ch, unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

