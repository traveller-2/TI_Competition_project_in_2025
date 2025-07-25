#include "mid_pid.h"

/******************************************************************
 * 函 数 说 明：速度环PID参数初始化
 * 函 数 形 参：
 * 函 数 返 回：
 * 作       者：LC
 * 备       注：
******************************************************************/
void pid_init(PID* pid, float p, float i, float d, float maxI, float maxOut, int target)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->max_change_i = maxI;
    pid->maxOutput = maxOut;
    pid->target = target;
}
/******************************************************************
 * 函 数 说 明：PID变化累计的参数清零
 * 函 数 形 参：
 * 函 数 返 回：
 * 作       者：LC
 * 备       注：
******************************************************************/
void pid_change_zero(PID* pid)
{
    pid->change_p = 0;
    pid->change_i = 0;
    pid->change_d = 0;
}

/******************************************************************
* 函 数 说 明：单级PID计算
 * 函 数 形 参：reference=目标位置	feedback=当前位置		
 * 函 数 返 回：PID输出值
 * 作       者：LC
 * 备       注：feedback = get_angle()
******************************************************************/
float pid_calc(PID *pid, float reference, float feedback)
{  
    pid->lastError=pid->error;//更新之前误差
    pid->error = reference - feedback;//获取新的误差
        
    //计算微分
    float dout = (pid->error - pid->lastError) * pid->kd;
    //计算比例
    float pout = pid->error * pid->kp;
    //计算积分
    pid->change_i += pid->error * pid->ki;

    //积分限幅
    if(pid->change_i > pid->max_change_i) pid->change_i = pid->max_change_i;
    else if(pid->change_i < -pid->max_change_i) pid->change_i = -pid->max_change_i;

    //计算输出
    pid->output = pout + dout + pid->change_i;	

    //输出限幅
    if(pid->output > pid->maxOutput) pid->output =   pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output = -pid->maxOutput;

    return pid->output; 
}

/******************************************************************
 * 函 数 说 明：串级PID的计算函数
 * 函 数 形 参：左到右依次是串级PID结构体,外环目标值,外环反馈值,内环反馈值
 * 函 数 返 回：
 * 作       者：
 * 备       注：
******************************************************************/
void pid_cascade_calc(CascadePID *pid, float outerRef, float outerFdb, int innerFdb)
{
    pid_calc(&pid->outer, outerRef, outerFdb); //计算外环	
    pid_calc(&pid->inner, pid->outer.output, innerFdb); //计算内环
    //内环输出就是串级PID的输出
    pid->output = pid->inner.output; 
}

