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


void encoder_init(void);
int get_encoder_count_l(void);
int get_encoder_count_r(void);
ENCODER_DIR get_encoder_dir_l(void);
ENCODER_DIR get_encoder_dir_r(void);
void encoder_update(void);

#endif