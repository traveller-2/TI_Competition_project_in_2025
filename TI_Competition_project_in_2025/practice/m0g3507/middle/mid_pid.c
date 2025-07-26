#include "mid_pid.h"

/************************************************
���ܣ���ʼ��PID������
������pid = ��Ӧpid�Ľṹ���ַ
      p = pid�ľ�̬kpֵ
      i = pid�ľ�̬kiֵ
      d = pid�ľ�̬kdֵ
      maxI = pid������I���ֵ�����������ۼ�ֵ
      maxOut = PID������ֵ
      target = Ŀ��ֵ
************************************************/
void pid_init(PID* pid, float p, float i, float d, float maxI, float maxOut, int target)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->max_change_i = maxI;
    pid->max_output = maxOut;
    pid->target = target;
}
/******************************************************************
 * �� �� ˵ ����PID�仯�ۼƵĲ�������
 * �� �� �� �Σ�pid=��Ӧ��PID��ַ
******************************************************************/
void pid_change_zero(PID* pid)
{
    pid->change_p = 0;
    pid->change_i = 0;
    pid->change_d = 0;
}

/****************************************************
���ܣ�����PID����
������pid = pid�Ĳ�������
     target = Ŀ��ֵ
     current = ��ǰֵ
���أ�PID�����Ľ��
****************************************************/
float pid_calc(PID *pid, float target, float current)
{
    //����һ�ε����ֵ���� ֮ǰ���last_error
    pid->last_error = pid->error;
    //��ȡ�µ���� = Ŀ��ֵ - ��ǰֵ
    pid->error = target - current;

    //�������P = Ŀ��ֵ��ʵ��ֵ֮������e
    float pout = pid->error;
    //�������I = ���e���ۼ�
    pid->change_i += pid->error;
    //����΢��D = ��ǰ���e - ֮ǰ�����last_e
    float dout = pid->error - pid->last_error;

    //����I ���Ʋ��ܳ����������ֵ
    if(pid->change_i > pid->max_change_i)
    {
      pid->change_i = pid->max_change_i;
    }
    else if(pid->change_i < -pid->max_change_i)
    {
      pid->change_i = -pid->max_change_i;
    }

    //�������PID_OUT = ��Kp x P��+ ��Ki x I��+��Kd x D��
    pid->output = (pid->kp * pout) + (pid->ki * pid->change_i) + (pid->kd * dout);

    //��� ���Ʋ��ܳ����������ֵ
    if(pid->output > pid->max_output) pid->output = pid->max_output;
    else if(pid->output < -pid->max_output) pid->output = -pid->max_output;

    //����PID����Ľ��
    return pid->output;
}

/******************************************************************
 * �� �� ˵ ��������PID�ļ��㺯��
 * �� �� �� �Σ����������Ǵ���PID�ṹ��,�⻷Ŀ��ֵ,�⻷����ֵ,�ڻ�����ֵ
 * �� �� �� �أ�
 * ��       �ߣ�
 * ��       ע��
******************************************************************/
void pid_cascade_calc(CascadePID *pid, float outerRef, float outerFdb, int innerFdb)
{
    pid_calc(&pid->outer, outerRef, outerFdb); //�����⻷	
    pid_calc(&pid->inner, pid->outer.output, innerFdb); //�����ڻ�
    //�ڻ�������Ǵ���PID�����
    pid->output = pid->inner.output; 
}

