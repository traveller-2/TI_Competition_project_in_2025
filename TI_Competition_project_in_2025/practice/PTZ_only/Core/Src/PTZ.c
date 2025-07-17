#include "PTZ.h"
#include "servo.h"
#include "cmsis_os.h"
#include "chassis.h"

// ========== �û��Զ������� ==========//
#define SERVO1_PWM_START     1200     // ���1��ʼPWM
#define SERVO1_PWM_END       1600    // ���1����PWM
#define SERVO2_PWM_START     1100     // ���2��ʼPWM
#define SERVO2_PWM_END       1900    // ���2����PWM
#define SERVO_STEP           50     // ɨ�貽������λ��PWM
#define SERVO_MOVE_TIME_MS   100    // ÿ����תʱ�䣨ms��
#define SERVO_DELAY_MS       10      // ÿ��ָ��֮���С��ʱ����ֹ���г�ͻ��

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
            osDelay(SERVO_MOVE_TIME_MS + SERVO_DELAY_MS);  // �ȴ�ת�����

        }
    }
	Servo_QueueControl(0, 1500, 1000);
	osDelay(10);
	Servo_QueueControl(1, 1500, 1000);
	osDelay(10);
	vTaskDelete(NULL);  // ɾ����ǰ����
}