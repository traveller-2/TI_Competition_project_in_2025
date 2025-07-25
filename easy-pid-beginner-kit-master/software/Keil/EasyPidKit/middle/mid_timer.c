#include "mid_timer.h"
#include "hw_encoder.h"
#include "mid_button.h"
#include "app_sys_mode.h"

// ����ö������
typedef enum {
    TASK_DISABLE=0, 
    TASK_ENABLE   
} Task_state;

static volatile Task_state tack_enable_flag = TASK_ENABLE;

//ʹ���������
void enable_task_interrupt(void)
{
	tack_enable_flag = TASK_ENABLE;
}
//ʧ���������
void disable_task_interrupt(void)
{
	tack_enable_flag = TASK_DISABLE;
}
//��ȡ����״̬
Task_state get_task_status(void)
{
	return tack_enable_flag;
}

void timer_init(void)
{
    //��ʱ���ж�
	NVIC_ClearPendingIRQ(TIMER_TICK_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_TICK_INST_INT_IRQN);
}

//����������������
void TIMER_TICK_INST_IRQHandler(void)
{
	//20ms�����жϴ���
	if( DL_TimerA_getPendingInterrupt(TIMER_TICK_INST) == DL_TIMER_IIDX_ZERO )
	{
		//����������
		if( get_functional_mode() != DISTANCE_FUNCTION )
		{
			encoder_update();
		}

		if( get_task_status() == TASK_ENABLE )
		{
			//����ɨ��+�¼�����
			flex_button_scan();
		}
	}
}