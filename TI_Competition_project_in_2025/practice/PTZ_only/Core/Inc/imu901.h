#ifndef __IMU901_H__
#define __IMU901_H__

#include <stdint.h>

typedef struct {
    float roll;
    float pitch;
    float yaw;
} imu901_attitude_t;

extern uint8_t imu_rx_byte;
extern imu901_attitude_t temp;
void imu901_light_init(void);
void imu901_uart_start(void);  // 串口中断接收启动函数
uint8_t imu901_light_receive(uint8_t ch);  // 每收到1字节调用它
imu901_attitude_t imu901_get_attitude(void);  // 获取最新姿态角

#endif
