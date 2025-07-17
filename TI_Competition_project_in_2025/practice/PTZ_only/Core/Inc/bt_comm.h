#ifndef __BT_COMM_H
#define __BT_COMM_H

#include "main.h"
#include "cmsis_os.h"

extern osThreadId_t btCommTaskHandle;
extern volatile	float angle;
extern int stop_flag;

// 帧头帧尾定义
#define FRAME_HEAD      		0xA5
#define MAP_HEAD 						0xd5

#define DISTANCE_ID     		0xD1
#define SPEED_ID       		 	0xD2
#define ANGLE_ID        		0xD3
#define LENGTH_ID       		0xD4
#define ANGLE_ID_DOWN   		0xD8
#define LENGTH_ID_DOWN  		0xD9
#define MAP_ID 							0xd6

#define DISTANCE_TAIL   		0x1D
#define SPEED_TAIL      		0x2D
#define ANGLE_TAIL      		0x3D
#define LENGTH_TAIL     		0x4D
#define ANGLE_TAIL_DOWN     0x8D
#define LENGTH_TAIL_DOWN    0x9D
#define MAP_TAIL 						0x6d

// 接口函数
void BT_Comm_Task(void *argument);
void send_float_frame(uint8_t header2, float value, uint8_t tail);
void WaitForBT_ReadySignal(void);
void send_r_theta_phi(float r, float theta, float phi);
void send_float_frame_PTZ(uint8_t header2, float value, uint8_t tail);

#endif
