#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include <stdint.h>

// ===== ȫ�ֱ����������� hc_sr04.c �ṩ��=====
extern volatile uint32_t ic_rising;
extern volatile uint32_t ic_falling;
extern volatile uint8_t  is_rising_edge;
extern volatile uint8_t  capture_done;
extern volatile uint32_t hcsr04_distance_mm;

// ===== �������� =====
void HCSR04_Trigger(void);
void HCSR04_Task(void *argument);

#endif /* __HC_SR04_H */
