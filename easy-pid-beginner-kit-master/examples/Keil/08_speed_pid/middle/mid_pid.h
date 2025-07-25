#ifndef	__MID_PID_H__
#define __MID_PID_H__


typedef struct
{
  float kp, ki, kd; 						      // 三个静态系数
  float change_p, change_i, change_d;	          // 三个动态参数
  float error, last_error; 						  // 误差、之前误差
  float max_change_i; 							  // 积分限幅
  float output, max_output; 				      // 输出、输出限幅
  int target;                                     // 目标
}PID;


//串级PID的结构体，包含两个单级PID
typedef struct
{
    PID inner; //内环
    PID outer; //外环
    float output; //串级输出
}CascadePID;

void pid_init(PID* pid, float p, float i, float d, float maxI, float maxOut, int target);
void pid_change_zero(PID* pid);
float pid_calc(PID *pid, float reference, float feedback);
void pid_cascade_calc(CascadePID *pid, float outerRef, float outerFdb, int innerFdb);

#endif
