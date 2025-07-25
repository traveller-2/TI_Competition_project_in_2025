#ifndef _HW_ENCODER_H_
#define _HW_ENCODER_H_

#include "ti_msp_dl_config.h"
#include <sys/cdefs.h>

#define Exti_IRQHandler     GROUP1_IRQHandler

typedef enum {
    FORWARD,  // 正向
    REVERSAL  // 反向
} ENCODER_DIR;

typedef struct {
    volatile long long temp_count;    //保存实时计数值
    int count;         //根据定时器时间更新的计数值
    ENCODER_DIR dir;            //旋转方向
} ENCODER_RES;


#define SPEED_ENCODER_VALUE_MAX ( -(SPEED_ENCODER_VALUE_MIN) )
#define SPEED_ENCODER_VALUE_MID 0
#define SPEED_ENCODER_VALUE_MIN (-100)

#define DISTANCE_ENCODER_VALUE_MAX ( -(DISTANCE_ENCODER_VALUE_MIN) )
#define DISTANCE_ENCODER_VALUE_MID 0
#define DISTANCE_ENCODER_VALUE_MIN (-360)

void encoder_init(void);
int get_encoder(void);
long long get_temp_encoder(void);
void encoder_update_repeat(void);
void encoder_update_continue(void);
#endif
