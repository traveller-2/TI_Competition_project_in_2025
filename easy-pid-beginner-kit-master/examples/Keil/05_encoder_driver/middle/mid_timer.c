#include "mid_timer.h"
#include "hw_encoder.h"
#include "mid_button.h"

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
		encoder_update();

		//����ɨ��+�¼�����
		flex_button_scan();
		
	}
}