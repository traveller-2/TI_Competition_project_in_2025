#ifndef	__MID_PID_H__
#define __MID_PID_H__


typedef struct
{
  float kp, ki, kd; 						      // ������̬ϵ��
  float change_p, change_i, change_d;	          // ������̬����
  float error, last_error; 						  // ��֮ǰ���
  float max_change_i; 							  // �����޷�
  float output, max_output; 				      // ���������޷�
  int target;                                     // Ŀ��
}PID;


//����PID�Ľṹ�壬������������PID
typedef struct
{
    PID inner; //�ڻ�
    PID outer; //�⻷
    float output; //�������
}CascadePID;

void pid_init(PID* pid, float p, float i, float d, float maxI, float maxOut, int target);
void pid_change_zero(PID* pid);
float pid_calc(PID *pid, float reference, float feedback);
void pid_cascade_calc(CascadePID *pid, float outerRef, float outerFdb, int innerFdb);

#endif
