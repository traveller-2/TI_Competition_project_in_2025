#include "hw_encoder.h"

uint32_t gpio_interrup;
ENCODER_RES motor_encoder;

//仅开启中断
void encoder_init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
    //定时器中断
	NVIC_ClearPendingIRQ(TIMER_TICK_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_TICK_INST_INT_IRQN);
}

/******************************************************************
 * 函 数 说 明：获取指定范围和指定时间内编码器的值
 * 函 数 形 参：
 * 函 数 返 回：返回对应的编码器值
 * 作       者：LC
 * 备       注：
******************************************************************/
int get_encoder(void)
{
	if( motor_encoder.count > SPEED_ENCODER_VALUE_MAX )  return SPEED_ENCODER_VALUE_MAX;
	if( motor_encoder.count < SPEED_ENCODER_VALUE_MIN ) return SPEED_ENCODER_VALUE_MIN;
	return motor_encoder.count;
}

/******************************************************************
 * 函 数 说 明：获取实时编码器的值
 * 函 数 形 参：
 * 函 数 返 回：返回对应的编码器值
 * 作       者：LC
 * 备       注：
******************************************************************/
long long get_temp_encoder(void)
{
	return motor_encoder.temp_count;
}

//编码器数据更新
//请间隔一定时间更新
void encoder_update_repeat(void)
{
	motor_encoder.count = motor_encoder.temp_count;
	//确定方向
	motor_encoder.dir = (motor_encoder.count > 0) ? FORWARD : REVERSAL;

	motor_encoder.temp_count = 0;//编码器计数值清零
}

void encoder_update_continue(void)
{
	motor_encoder.count = motor_encoder.count + motor_encoder.temp_count;
	//确定方向
	motor_encoder.dir = (motor_encoder.count > 0) ? FORWARD : REVERSAL;

}

/*******************************************************
函数功能：外部中断模拟编码器信号
入口函数：无
返回  值：无
***********************************************************/
void Exti_IRQHandler(void)
{
	//获取中断信号
	gpio_interrup = DL_GPIO_getEnabledInterruptStatus(GPIOB,GPIO_ENCODER_PIN_A_PIN|GPIO_ENCODER_PIN_B_PIN);
	//encoderA
	if((gpio_interrup & GPIO_ENCODER_PIN_A_PIN)==GPIO_ENCODER_PIN_A_PIN)
	{
		if(!DL_GPIO_readPins(GPIOB,GPIO_ENCODER_PIN_B_PIN))
		{
			motor_encoder.temp_count--;
		}
		else
		{
			motor_encoder.temp_count++;
		}
	}
	else if((gpio_interrup & GPIO_ENCODER_PIN_B_PIN)==GPIO_ENCODER_PIN_B_PIN)
	{
		if(!DL_GPIO_readPins(GPIOB,GPIO_ENCODER_PIN_A_PIN))
		{
			motor_encoder.temp_count++;
		}
		else
		{
			motor_encoder.temp_count--;
		}
	}
	//清除状态
	DL_GPIO_clearInterruptStatus(GPIOB,GPIO_ENCODER_PIN_A_PIN|GPIO_ENCODER_PIN_B_PIN);
}

