#ifndef _HW_KEY_H_
#define _HW_KEY_H_

#include "ti_msp_dl_config.h"

typedef struct {
    unsigned int up : 1; // ʹ��λ�ֶ�����ʡ�ռ�
    unsigned int left : 1;
    unsigned int right : 1;
    unsigned int down : 1;
} KEY_STATUS;

KEY_STATUS key_scan(void);

#endif
