#include "hw_motor.h"

// ����fi���ŵ�PWM�Ƚ�ֵ
static void set_fi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,dat,GPIO_PWM_MOTOR_C1_IDX);
}
// ����bi���ŵ�PWM�Ƚ�ֵ
static void set_bi(uint16_t dat)
{
    DL_TimerG_setCaptureCompareValue(PWM_MOTOR_INST,dat,GPIO_PWM_MOTOR_C0_IDX);
}

// ���������PWM���ֵ
// ���ڲ�ʹ��
static void restrict_pwm_max_value(uint16_t* value)
{
    if( *value > MOTOR_PWM_MAX ) 
    {
        *value = MOTOR_PWM_MAX;
    }
}

// ���õ���ٶ�
void set_motor(uint16_t fi_value, uint16_t bi_value)
{
		restrict_pwm_max_value(&fi_value);
		restrict_pwm_max_value(&bi_value);
	
    set_fi(fi_value);
    set_bi(bi_value);
}

// ���ֹͣ
void stop_motor(void)
{
    set_motor(0,0);
}
