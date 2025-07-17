#include "PTZ.h"
#include "servo.h"
#include "cmsis_os.h"
#include "chassis.h"

// ========== 用户自定义配置 ==========//
#define SERVO1_PWM_START     1200     // 舵机1起始PWM
#define SERVO1_PWM_END       1600    // 舵机1结束PWM
#define SERVO2_PWM_START     1100     // 舵机2起始PWM
#define SERVO2_PWM_END       1900    // 舵机2结束PWM
#define SERVO_STEP           50     // 扫描步长，单位：PWM
#define SERVO_MOVE_TIME_MS   100    // 每步旋转时间（ms）
#define SERVO_DELAY_MS       10      // 每次指令之间的小延时（防止队列冲突）

void PTZ_TASK(void *argument)
{
	Servo_QueueControl(0, 1500, 1000);
	osDelay(10);
	Servo_QueueControl(1, 1500, 1000);
	osDelay(10);
	for(;;)
	{
		if(flag == 1)
		{
			break;
		}
		osDelay(500);
	}
	
	for (int pwm1 = SERVO1_PWM_START; pwm1 <= SERVO1_PWM_END; pwm1 += SERVO_STEP)
    {
        for (int pwm2 = SERVO2_PWM_START; pwm2 <= SERVO2_PWM_END; pwm2 += SERVO_STEP)
        {
            Servo_QueueControl(0, pwm1, SERVO_MOVE_TIME_MS);
            osDelay(SERVO_DELAY_MS);

            Servo_QueueControl(1, pwm2, SERVO_MOVE_TIME_MS);
            osDelay(SERVO_MOVE_TIME_MS + SERVO_DELAY_MS);  // 等待转动完成

        }
    }
	Servo_QueueControl(0, 1500, 1000);
	osDelay(10);
	Servo_QueueControl(1, 1500, 1000);
	osDelay(10);
	vTaskDelete(NULL);  // 删除当前任务
}