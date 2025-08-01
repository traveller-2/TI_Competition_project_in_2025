#include "mid_timer.h"
#include "hw_encoder.h"
#include "mid_button.h"
#include "app_sys_mode.h"

// 定义枚举类型
typedef enum {
    TASK_DISABLE=0, 
    TASK_ENABLE   
} Task_state;

static volatile Task_state tack_enable_flag = TASK_ENABLE;

//使能任务调度
void enable_task_interrupt(void)
{
	tack_enable_flag = TASK_ENABLE;
}
//失能任务调度
void disable_task_interrupt(void)
{
	tack_enable_flag = TASK_DISABLE;
}
//获取任务状态
Task_state get_task_status(void)
{
	return tack_enable_flag;
}

void timer_init(void)
{
    //定时器中断
	NVIC_ClearPendingIRQ(TIMER_TICK_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_TICK_INST_INT_IRQN);
}

//电机编码器脉冲计数
void TIMER_TICK_INST_IRQHandler(void)
{
	//20ms归零中断触发
	if( DL_TimerA_getPendingInterrupt(TIMER_TICK_INST) == DL_TIMER_IIDX_ZERO )
	{
		//编码器更新
		if( get_functional_mode() != DISTANCE_FUNCTION )
		{
			encoder_update();
		}

		if( get_task_status() == TASK_ENABLE )
		{
			//按键扫描+事件管理
			flex_button_scan();
		}
	}
}