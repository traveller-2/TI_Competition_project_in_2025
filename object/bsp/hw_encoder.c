#include "hw_encoder.h"
#include "ti_msp_dl_config.h"

static ENCODER_RES motor_encoder_l;
static ENCODER_RES motor_encoder_r;
//编码器初始化
void encoder_init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	
}

//获取编码器的值
int get_encoder_count_l(void)
{
	return motor_encoder_l.count;
}
int get_encoder_count_r(void)
{
	return motor_encoder_r.count;
}
//获取编码器的方向
ENCODER_DIR get_encoder_dir_l(void)
{
	return motor_encoder_l.dir;
}
ENCODER_DIR get_encoder_dir_r(void)
{
	return motor_encoder_r.dir;
}

//编码器数据更新
//请间隔一定时间更新
void encoder_update(void)
{
	motor_encoder_l.count += motor_encoder_l.temp_count;

	//确定方向
	motor_encoder_l.dir = ( motor_encoder_l.count >= 0 ) ? FORWARD : REVERSAL;

	motor_encoder_l.temp_count = 0;//编码器计数值清零
	
	
	motor_encoder_r.count += motor_encoder_r.temp_count;

	//确定方向
	motor_encoder_r.dir = ( motor_encoder_r.count >= 0 ) ? FORWARD : REVERSAL;

	motor_encoder_r.temp_count = 0;//编码器计数值清零
}

//外部中断处理函数
void GROUP1_IRQHandler(void)
{
	uint32_t gpio_status_l = 0;
	uint32_t gpio_status_r = 0;

	//获取中断信号情况
	gpio_status_l = DL_GPIO_getEnabledInterruptStatus(GPIO_ECODER_PORT, GPIO_ECODER_LEFT_A_PIN | GPIO_ECODER_LEFT_B_PIN);
	//编码器A相上升沿触发
	if((gpio_status_l & GPIO_ECODER_LEFT_A_PIN) == GPIO_ECODER_LEFT_B_PIN)
	{
		//如果在A相上升沿下，B相为低电平
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_B_PIN))
		{
			motor_encoder_l.temp_count--;
		}
		else
		{
			motor_encoder_l.temp_count++;
		}
	}//编码器B相上升沿触发
	else if((gpio_status_l & GPIO_ECODER_LEFT_B_PIN)==GPIO_ECODER_LEFT_B_PIN)
	{
		//如果在B相上升沿下，A相为低电平
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_A_PIN))
		{
			motor_encoder_l.temp_count++;
		}
		else
		{
			motor_encoder_l.temp_count--;
		}
	}
	//清除状态
	DL_GPIO_clearInterruptStatus(GPIO_ECODER_PORT,GPIO_ECODER_LEFT_A_PIN|GPIO_ECODER_LEFT_B_PIN);
	


	//获取中断信号情况
	gpio_status_r = DL_GPIO_getEnabledInterruptStatus(GPIO_ECODER_PORT, GPIO_ECODER_RIGHT_A_PIN | GPIO_ECODER_RIGHT_B_PIN);
	//编码器A相上升沿触发
	if((gpio_status_r & GPIO_ECODER_RIGHT_A_PIN) == GPIO_ECODER_RIGHT_B_PIN)
	{
		//如果在A相上升沿下，B相为低电平
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_B_PIN))
		{
			motor_encoder_r.temp_count--;
		}
		else
		{
			motor_encoder_r.temp_count++;
		}
	}//编码器B相上升沿触发
	else if((gpio_status_r & GPIO_ECODER_RIGHT_B_PIN)==GPIO_ECODER_RIGHT_B_PIN)
	{
		//如果在B相上升沿下，A相为低电平
		if(!DL_GPIO_readPins(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_A_PIN))
		{
			motor_encoder_r.temp_count++;
		}
		else
		{
			motor_encoder_r.temp_count--;
		}
	}
	//清除状态
	DL_GPIO_clearInterruptStatus(GPIO_ECODER_PORT,GPIO_ECODER_RIGHT_A_PIN|GPIO_ECODER_RIGHT_B_PIN);
}