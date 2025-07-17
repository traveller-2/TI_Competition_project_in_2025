#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "main.h"
#include "tim.h"
#include "cmsis_os.h"
#include <stdint.h>

// ===== 全局变量声明（由 hc_sr04.c 提供）=====
extern volatile uint32_t ic_rising;
extern volatile uint32_t ic_falling;
extern volatile uint8_t  is_rising_edge;
extern volatile uint8_t  capture_done;
extern volatile uint32_t hcsr04_distance_mm;

// ===== 函数声明 =====
void HCSR04_Trigger(void);
void HCSR04_Task(void *argument);

#endif /* __HC_SR04_H */
