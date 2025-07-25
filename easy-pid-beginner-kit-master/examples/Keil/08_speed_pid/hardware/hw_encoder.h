#ifndef _HW_ENCODER_H_
#define _HW_ENCODER_H_

#include "ti_msp_dl_config.h"

typedef enum {
    FORWARD,  // ����
    REVERSAL  // ����
} ENCODER_DIR;

typedef struct {
    volatile long long temp_count; //����ʵʱ����ֵ
    int count;         						 //���ݶ�ʱ��ʱ����µļ���ֵ
    ENCODER_DIR dir;            	 //��ת����
} ENCODER_RES;

#define SPEED_ENCODER_VALUE_MAX ( -(SPEED_ENCODER_VALUE_MIN) )
#define SPEED_ENCODER_VALUE_MID 0
#define SPEED_ENCODER_VALUE_MIN (-100)

#define DISTANCE_ENCODER_VALUE_MAX ( -(DISTANCE_ENCODER_VALUE_MIN) )
#define DISTANCE_ENCODER_VALUE_MID 0
#define DISTANCE_ENCODER_VALUE_MIN (-360)

void encoder_init(void);
int get_encoder_count(void);
ENCODER_DIR get_encoder_dir(void);
void encoder_update(void);

#endif
