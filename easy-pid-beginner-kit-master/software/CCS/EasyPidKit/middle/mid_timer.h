#ifndef	__MID_TIMER_H__
#define __MID_TIMER_H__

#include "ti_msp_dl_config.h"

#define Timer_IRQHandler    TIMER_TICK_INST_IRQHandler

void enable_task_interrupt(void);
void disable_task_interrupt(void);

#endif
