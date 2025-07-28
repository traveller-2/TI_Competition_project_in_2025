#include "motorDriver.h"
#include "ti_msp_dl_config.h"

// 设定 PWM 周期常数
#define PWM_PERIOD_COUNT 100  // 与 SysConfig 中保持一致

// 左电机宏定义（根据你的实际引脚修改）
#define TIMER_LEFT         TIMERA0
#define PWM_CH_LEFT        DL_TIMER_CC_INDEX_0
#define GPIO_LEFT_PORT     GPIO_PORT_P1
#define GPIO_LEFT_PIN      GPIO_PIN_0  // 假设为P1.0

// 右电机宏定义（根据你的实际引脚修改）
#define TIMER_RIGHT        TIMERA1
#define PWM_CH_RIGHT       DL_TIMER_CC_INDEX_0
#define GPIO_RIGHT_PORT    GPIO_PORT_P2
#define GPIO_RIGHT_PIN     GPIO_PIN_0  // 假设为P2.0

// speed范围 -100~100
void Motor_SetSpeed(int8_t left, int8_t right)
{
		right = -right;
    // 左电机控制
    if (left > 0) {
        // 正转
        DL_GPIO_clearPins(GPIO_MOTOR_PIN_MOTORL_PORT, GPIO_MOTOR_PIN_MOTORL_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORL_INST, PWM_PERIOD_COUNT-left,
                                         DL_TIMER_CC_1_INDEX);  // left ∈ 0~100
    } else {
        // 反转
        DL_GPIO_setPins(GPIO_MOTOR_PIN_MOTORL_PORT, GPIO_MOTOR_PIN_MOTORL_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_MOTORL_INST, -left,
                                         DL_TIMER_CC_1_INDEX);  // 100 - abs(left)
    }

    // 右电机控制
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
