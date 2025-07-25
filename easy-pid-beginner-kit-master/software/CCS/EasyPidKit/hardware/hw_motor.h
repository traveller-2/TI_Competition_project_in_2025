#ifndef _HW_MOTOR_H_
#define _HW_MOTOR_H_

#include "ti_msp_dl_config.h"




// void set_fi(uint16_t dat);
// void set_bi(uint16_t dat);
void set_motor(uint16_t fi_value, uint16_t bi_value);
void stop_motor(void);
#endif
