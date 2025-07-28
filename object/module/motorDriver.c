#include "motorDriver.h"
#include "ti_msp_dl_config.h"

// �趨 PWM ���ڳ���
#define PWM_PERIOD_COUNT 100  // �� SysConfig �б���һ��

// �����궨�壨�������ʵ�������޸ģ�
#define TIMER_LEFT         TIMERA0
#define PWM_CH_LEFT        DL_TIMER_CC_INDEX_0
#define GPIO_LEFT_PORT     GPIO_PORT_P1
#define GPIO_LEFT_PIN      GPIO_PIN_0  // ����ΪP1.0

// �ҵ���궨�壨�������ʵ�������޸ģ�
#define TIMER_RIGHT        TIMERA1
#define PWM_CH_RIGHT       DL_TIMER_CC_INDEX_0
#define GPIO_RIGHT_PORT    GPIO_PORT_P2
#define GPIO_RIGHT_PIN     GPIO_PIN_0  // ����ΪP2.0

// speed��Χ -100~100
void Motor_SetSpeed(int8_t left, int8_t right)
{
		right = -right;
    // ��������
    if (left > 0) {
        // ��ת
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_MOTORL_PORT, GPIO_MOTOR_PIN_MOTORL_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORL_INST, PWM_PERIOD_COUNT-left,
                                         DL_TIMER_CC_1_INDEX);  // left �� 0~100
    } else {
        // ��ת
        DL_GPIO_setPins(GPIO_MOTOR_PIN_MOTORL_PORT, GPIO_MOTOR_PIN_MOTORL_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORL_INST, -left,
                                         DL_TIMER_CC_1_INDEX);  // 100 - abs(left)
    }

    // �ҵ������
    if (right > 0) {
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_MOTORR_PORT, GPIO_MOTOR_PIN_MOTORR_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORR_INST, PWM_PERIOD_COUNT-right,
                                         GPIO_PWM_MOTORR_C0_IDX);
    } else {
        DL_GPIO_setPins(GPIO_MOTOR_PIN_MOTORR_PORT, GPIO_MOTOR_PIN_MOTORR_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORR_INST, -right,
                                         GPIO_PWM_MOTORR_C0_IDX);
    }
}
