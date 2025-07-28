#include "hw_encoder.h"
#include "ti_msp_dl_config.h"

static ENCODER_RES motor_encoder_l;
static ENCODER_RES motor_encoder_r;
//��������ʼ��
void encoder_init(void)
{
	//�����������ⲿ�ж�
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	
}

//��ȡ��������ֵ
int get_encoder_count_l(void)
{
	return motor_encoder_l.count;
}
int get_encoder_count_r(void)
{
	return motor_encoder_r.count;
}
//��ȡ�������ķ���
ENCODER_DIR get_encoder_dir_l(void)
{
	return motor_encoder_l.dir;
}
ENCODER_DIR get_encoder_dir_r(void)
{
	return motor_encoder_r.dir;
}

//���������ݸ���
//����һ��ʱ�����
void encoder_update(void)
{
	motor_encoder_l.count = motor_encoder_l.temp_count;

	//ȷ������
	motor_encoder_l.dir = ( motor_encoder_l.count >= 0 ) ? FORWARD : REVERSAL;

	motor_encoder_l.temp_count = 0;//����������ֵ����
	
	
	motor_encoder_r.count = motor_encoder_r.temp_count;

	//ȷ������
	motor_encoder_r.dir = ( motor_encoder_r.count >= 0 ) ? FORWARD : REVERSAL;

	motor_encoder_r.temp_count = 0;//����������ֵ����
}

//�ⲿ�жϴ�����
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_status_l = 0;
	uint32_t gpio_status_r = 0;

	//��ȡ�ж��ź����
	gpio_status_l = DL_GPIO_getEnabledInterruptStatus(GPIO_ECODER_PORT, GPIO_ECODER_LEFT_A_PIN | GPIO_ECODER_LEFT_B_PIN);
	//������A�������ش���
	if((gpio_status_l & GPIO_ECODER_LEFT_A_PIN) == GPIO_ECODER_LEFT_B_PIN)
	{
		//�����A���������£�B��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_B_PIN))
		{
			motor_encoder_l.temp_count--;
		}
		else
		{
			motor_encoder_l.temp_count++;
		}
	}//������B�������ش���
	else if((gpio_status_l & GPIO_ECODER_LEFT_B_PIN)==GPIO_ECODER_LEFT_B_PIN)
	{
		//�����B���������£�A��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_A_PIN))
		{
			motor_encoder_l.temp_count++;
		}
		else
		{
			motor_encoder_l.temp_count--;
		}
	}
	//���״̬
	DL_GPIO_clearInterruptStatus(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_A_PIN|GPIO_ECODER_LEFT_B_PIN);
	


	//��ȡ�ж��ź����
	gpio_status_r = DL_GPIO_getEnabledInterruptStatus(GPIO_ECODER_PORT, GPIO_ECODER_RIGHT_A_PIN | GPIO_ECODER_RIGHT_B_PIN);
	//������A�������ش���
	if((gpio_status_r & GPIO_ECODER_RIGHT_A_PIN) == GPIO_ECODER_RIGHT_B_PIN)
	{
		//�����A���������£�B��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_B_PIN))
		{
			motor_encoder_r.temp_count--;
		}
		else
		{
			motor_encoder_r.temp_count++;
		}
	}//������B�������ش���
	else if((gpio_status_r & GPIO_ECODER_RIGHT_B_PIN)==GPIO_ECODER_RIGHT_B_PIN)
	{
		//�����B���������£�A��Ϊ�͵�ƽ
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_A_PIN))
		{
			motor_encoder_r.temp_count++;
		}
		else
		{
			motor_encoder_r.temp_count--;
		}
	}
	//���״̬
	DL_GPIO_clearInterruptStatus(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_A_PIN|GPIO_ECODER_RIGHT_B_PIN);
}