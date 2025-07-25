#include "hw_encoder.h"

static ENCODER_RES motor_encoder;

//��������ʼ��
void encoder_init(void)
{
	//�����������ⲿ�ж�
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

//��ȡ��������ֵ
int get_encoder_count(void)
{
	return motor_encoder.count;
}
//��ȡ�������ķ���
ENCODER_DIR get_encoder_dir(void)
{
	return motor_encoder.dir;
}

/******************************************************************
 * �� �� ˵ ������ȡʵʱ��������ֵ
 * �� �� �� �Σ�
 * �� �� �� �أ����ض�Ӧ�ı�����ֵ
 * ��       �ߣ�LC
 * ��       ע��
******************************************************************/
long long get_temp_encoder(void)
{
	return motor_encoder.temp_count;
}

//���������ݸ���
//����һ��ʱ�����
void encoder_update(void)
{
	motor_encoder.count = motor_encoder.temp_count;

	//ȷ������
	motor_encoder.dir = ( motor_encoder.count >= 0 ) ? FORWARD : REVERSAL;
	
	motor_encoder.temp_count = 0;//����������ֵ����
}


//�ⲿ�жϴ�����
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_status;
	
	//��ȡ�ж��ź����
	gpio_status = DL_GPIO_getEnabledInterruptStatus(GPIO_ENCODER_PORT, GPIO_ENCODER_PIN_A_PIN | GPIO_ENCODER_PIN_B_PIN);
	//������A�������ش��� 
	if((gpio_status & GPIO_ENCODER_PIN_A_PIN) == GPIO_ENCODER_PIN_A_PIN)
	{
		//�����A���������£�B��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_B_PIN))
		{
			motor_encoder.temp_count--;
		}
		else
		{
			motor_encoder.temp_count++;
		}
	}//������B�������ش��� 
	else if((gpio_status & GPIO_ENCODER_PIN_B_PIN)==GPIO_ENCODER_PIN_B_PIN)
	{
		//�����B���������£�A��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_A_PIN))
		{
			motor_encoder.temp_count++;
		}
		else
		{
			motor_encoder.temp_count--;
		}
	}
	//���״̬
	DL_GPIO_clearInterruptStatus(GPIO_ENCODER_PORT,GPIO_ENCODER_PIN_A_PIN|GPIO_ENCODER_PIN_B_PIN);
}

