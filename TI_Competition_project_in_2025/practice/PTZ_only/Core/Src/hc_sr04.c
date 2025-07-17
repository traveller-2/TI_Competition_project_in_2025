#include "hc_sr04.h"
#include "cmsis_os.h"
#include "tim.h"


volatile uint32_t ic_rising = 0;
volatile uint32_t ic_falling = 0;
volatile uint8_t  is_rising_edge = 1;
volatile uint8_t  capture_done = 0;

volatile uint32_t hcsr04_distance_mm = 0;

extern TIM_HandleTypeDef htim1;

void HCSR04_Task(void *argument)
{
    uint32_t distance = 0;

    while (1)
    {
        HCSR04_Trigger();

        // �ȴ�������ɻ�ʱ
        uint32_t timeout = 0;
        while (!capture_done && timeout < 300)
        {
            osDelay(1);
            timeout++;
        }

        if (capture_done)
				{
						uint32_t diff = (ic_falling >= ic_rising) ? (ic_falling - ic_rising) : (0xFFFF - ic_rising + ic_falling);

						// ��Чʱ�䷶Χ����Լ 2cm ~ 400cm��
						if (diff > 100 && diff < 25000)
						{
								distance = (diff * 0.034f) / 2;
						}
						else
						{
								distance = 555; // �쳣��Ĭ��
						}

						capture_done = 0;
				}

				else
				{
					distance = 55;
				}
				hcsr04_distance_mm = distance ; // ��������
        osDelay(200); // ÿ 500ms ��һ��
    }
}


void HCSR04_Trigger(void)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);  // PE12 = TRIG
    uint32_t delay = HAL_RCC_GetHCLKFreq() / 1000000 * 10; // 10us ��ʱ
    for(uint32_t i = 0; i < delay; i++) __NOP();
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
}
