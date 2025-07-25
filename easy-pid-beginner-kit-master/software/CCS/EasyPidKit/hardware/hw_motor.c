#include "hw_motor.h"

static void set_fi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,dat,GPIO_PWM_MOTOR_C1_IDX);
}
static void set_bi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,dat,GPIO_PWM_MOTOR_C0_IDX);
}


void set_motor(uint16_t fi_value, uint16_t bi_value)
{

    set_fi(fi_value);
    set_bi(bi_value);
}

void stop_motor(void)
{
    set_motor(0,0);
}