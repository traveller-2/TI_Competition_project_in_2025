#include "chassis.h"
#include "motor_iic.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include <math.h>
#include "hc_sr04.h"
#include "imu901.h"
#include "servo.h"
#include "bsp_vl53l0x.h"
#include "imu901.h"
#include "bt_comm.h"

// ��ʼ��λ�� PID ������
PID_Controller posPID = {
		.target = 0,
		.Kp = 8.5f,
		.Ki = 0.0f,
		.Kd = -0.83f
};

PID_Controller distancePID = {
    .target = 35.0f,   // ����Ŀ�꣬���� 20cm
    .Kp = 5.5f,        // �ɵ�����Ӧ�ٶ�
    .Ki = 0.0f,       // �ɵ�������ƫ��
    .Kd = 0.0f         // �ɵ���������
};

//PID_Controller yaw_pid = {
//    .target = 0,
//    .Kp = 1.0f,
//    .Ki = 0.0f,
//    .Kd = 0.0f
//};

// FreeRTOS ������
osThreadId_t chassisTaskHandle;

// GPIO�˿ں����Ŷ��壨CubeMX���ã�
#define GRAY_GPIO_PORT_L_M   	GPIOF
#define GRAY_LEFT2_PIN   			GPIO_PIN_13
#define GRAY_LEFT1_PIN   			GPIO_PIN_14
#define GRAY_MID_PIN     			GPIO_PIN_15
#define GRAY_GPIO_PORT_R   		GPIOG
#define GRAY_RIGHT1_PIN  			GPIO_PIN_0
#define GRAY_RIGHT2_PIN  			GPIO_PIN_1


extern int flag0;

int flag = 0;
static uint8_t last_valid_gray[5] = {0};  // ������һ����Ч״̬

//���ٶȿ���
static int8_t current_left_speed = 0;
static int8_t current_right_speed = 0;

static int8_t target_left_speed = 0;
static int8_t target_right_speed = 0;

float weights[NUM_SENSORS] = {-2.5, -1, 0, 1, 2.5};
static int8_t baseSpeed = 18;
uint8_t gray[5] = {0};
float posError;
float distance_output;

int cnt = 0;

//typedef struct {
//  float target;      // Ŀ��ֵ
//  float current;     // ��ǰֵ
//  float Kp, Ki, Kd; // PID����
//  float integral;    // ������
//  float last_error;  // �ϴ����
//} PID_Controller;



// FreeRTOS ����ѭ������
void ChassisFollowTask(void *argument)
{

    Motor_SetSpeed_Async(40, 40, 0, 0);
    osDelay(300);

    while (1)
    {		
				
        // ��ȡ��̬�ǣ�pitch��
        imu901_attitude_t att = imu901_get_attitude();
        float pitch = -(att.roll - 7.7f);

        // ���������ϣ�Ŀ�����ά�� 28cm�����̫Զ�򲻿���
        if (hcsr04_distance_mm < 35)
        {
            distance_output = Distance_PID_Update(&distancePID, (float)hcsr04_distance_mm);
        }
        else
        {
            distance_output = 0.0f;
        }

        // ��ȡ�Ҷȴ�����������ѭ�����
        Read_All_GraySensors(gray);
        posError = Calculate_Error();
        float turn_output = PID_Update(&posPID, posError);

        // ִ�е��̿���
        Motor_Control(turn_output);

        // ѭ�����ڣ�10ms
        osDelay(10);

        // �ж��Ƿ񵽴�ֹͣ������
        // - ��/�Ҳ�ȫ��ƫ�루�������յ㣩
        // - �������� && ��ǰ�����£�pitch < 5�㣩

        if(((gray[0]==0&&gray[1]==0&&gray[2]==0&&gray[3]==0)||	(gray[4]==0&&gray[1]==0&&gray[2]==0&&gray[3]==0))&&( fabs(pitch) <= 5)&&stop_flag == 1)
        {
            Motor_SetSpeed_Async(0, 0, 0, 0);  // ֹͣ����
            osDelay(200);  // ������ʱ�ȶ�
            flag = 1;      // ������һ���׶εı�־
            break;         // �˳����񣨻������������
        }
				
//				//��������
//				if((gray[0]==1&&gray[1]==1&&gray[2]==1&&gray[3]==1&&gray[4]==1)&&( fabs(pitch) <= 5)&&hcsr04_distance_mm < 35)
//				{
//					for(int i=0;i<20;i=i+4)
//					{
//						Motor_SetSpeed_Async(20+i, 20, 0, 0);
//						osDelay(100);
//					}
//					for(int i=20;i>0;i=i-4)
//					{
//						Motor_SetSpeed_Async(20+i, 20, 0, 0);
//						osDelay(100);
//					}
//					
////					Motor_SetSpeed_Async(20, 20, 0, 0);
////					osDelay(500);
//					
//						for(int i=0;i<20;i=i+4)
//					{
//						Motor_SetSpeed_Async(20, 20+i, 0, 0);
//						osDelay(150);
//					}
//					for(int i=20;i>=0;i=i-4)
//					{
//						Motor_SetSpeed_Async(20, 20+i, 0, 0);
//						osDelay(150);
//					}
//					
//					while(1)
//					{	Read_All_GraySensors(gray);
//						
//						if(gray[2]==0||gray[3]==0||gray[4]==0)
//						{
//							break;
//						}
//						osDelay(20);
//					}
////					float target_yaw = att.yaw - 45.0f;       // ��Ҫ�ĽǶȣ�Ŀ�곯��
////					yaw_pid.target = 0;  											// Ŀ�������Ϊ0
////					uint32_t start = HAL_GetTick();
////					while (HAL_GetTick() - start < 2000)//����ѭ������ʱ��
////					{
////							imu901_attitude_t att = imu901_get_attitude();
////							float yaw_now = att.yaw;

////							// ����Ƕ�������Ƕ��ƻ����⣩
////							float yaw_error = target_yaw - yaw_now;
////							if (yaw_error > 180.0f) yaw_error -= 360.0f;
////							if (yaw_error < -180.0f) yaw_error += 360.0f;

////							// ���� PID �����ת��������������ֲ��٣�
////							float turn_output = PID_Update(&yaw_pid, yaw_error);

////							// �����������ٶȣ����ٿ��ƣ�
////							float left_speed = baseSpeed + turn_output;
////							float right_speed = baseSpeed - turn_output;
////							
////						  left_speed = (left_speed < 0) ? 0 : (left_speed > 60) ? 60 : left_speed;
////							right_speed = (right_speed < 0) ? 0 : (right_speed > 60) ? 60 : right_speed;
////							// �����ٶ�ָ��
////							Motor_SetSpeed_Async(left_speed, right_speed, 0, 0);

////							osDelay(20);
////					}
////					
////					target_yaw = att.yaw + 75.0f;
////					while(1)
////					{
////							imu901_attitude_t att = imu901_get_attitude();
////							float yaw_now = att.yaw;

////							// ����Ƕ�������Ƕ��ƻ����⣩
////							float yaw_error = target_yaw - yaw_now;
////							if (yaw_error > 180.0f) yaw_error -= 360.0f;
////							if (yaw_error < -180.0f) yaw_error += 360.0f;

////							// ���� PID �����ת��������������ֲ��٣�
////							float turn_output = PID_Update(&yaw_pid, yaw_error);

////							// �����������ٶȣ����ٿ��ƣ�
////							float left_speed = baseSpeed + turn_output;
////							float right_speed = baseSpeed - turn_output;
////							
////						  left_speed = (left_speed < 0) ? 0 : (left_speed > 60) ? 60 : left_speed;
////							right_speed = (right_speed < 0) ? 0 : (right_speed > 60) ? 60 : right_speed;
////							// �����ٶ�ָ��
////							Motor_SetSpeed_Async(left_speed, right_speed, 0, 0);
////							
////							if(gray[2]==0||gray[3]==0||gray[4]==0)
////							{
////								break;
////							}
////							osDelay(20);
////					}
//				}ע�ͱ�ǣ��������������
				
    }		
		vTaskDelete(NULL);  // ɾ����ǰ����
}

		
/***********************************˵��*********************************************
	*Servo_Init();                    // ��һ������ʼ��
	*Servo_SetID(0, 1);               // �ڶ��������Σ����޸�ID��ÿ�����Ψһ��
	*Servo_SetMode(1, 1);             // �����������ö��ģʽ����270�ȣ�
	*
	* ���ƶ�����
	*Servo_QueueControl(1, 1500, 1000);
	*Servo_QueueControl(2, 2000, 1000);
	*
	* ��ѡ����ȡ����Ƕ�
	*char buffer[20];
	*Servo_ReadAngle(1, buffer, 100);
	************************************************************************************/

// ���������ζ�ȡ�Ҷ�ֵ
static void Read_All_GraySensors(uint8_t *gray)
{
    static uint8_t last_valid_gray[5] = {0};
    static uint8_t last_pattern = 0;

    // ��ǰ��ȡ
    uint8_t raw[5];
    raw[0] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_LEFT2_PIN);   // ��2
    raw[1] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_LEFT1_PIN);   // ��1
    raw[2] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_MID_PIN);     // ��
    raw[3] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_R, GRAY_RIGHT1_PIN);    // ��1
    raw[4] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_R, GRAY_RIGHT2_PIN);    // ��2

    // ����Ƿ�ȫΪ�ף�ȫ��Ϊ1��
    uint8_t all_white = 1;
    for (int i = 0; i < 5; i++)
    {
        if (raw[i] == 0)
        {
            all_white = 0;
            break;
        }
    }

    if (all_white)
    {
        // �ж���һ���Ƿ��ǡ���һ�൥��Ϊ0��
        //uint8_t sum = last_valid_gray[0] + last_valid_gray[1] + last_valid_gray[2] + last_valid_gray[3] + last_valid_gray[4];
        if (last_valid_gray[0] == 0 || last_valid_gray[4] == 0)
        {
            // ����ϴ�ֻ��һ��0�����ֲ���
            for (int i = 0; i < 5; i++)
            {
                gray[i] = last_valid_gray[i];
            }
            return;
        }
    }

    // ������������ gray
    for (int i = 0; i < 5; i++)
    {
        gray[i] = raw[i];
        last_valid_gray[i] = raw[i];  // ����Ϊ���һ����Ч�Ҷ�ֵ
    }
}


//// �����߼������ֲ��٣�static void FollowLine(const uint8_t *gray)
//static void FollowLine(const uint8_t *gray)
//{
//    static uint8_t last_gray[5] = {0};  // ��һ�λҶ�״̬

//    // ״̬�����־λ
//    uint8_t detected = 0;

//    // ����Ƿ����κ�һ��������ʶ�𵽺���
//    for (int i = 0; i < 5; i++)
//    {
//        if (gray[i] == GRAY_BLACK)
//        {
//            detected = 1;
//            break;
//        }
//    }

//    if (detected)
//    {
//        // ����ѭ������
//        if (gray[2] == GRAY_BLACK && gray[1] == GRAY_WHITE && gray[3] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(30, 30, 0, 0);
//        }
//        else if (gray[3] == GRAY_BLACK && gray[4] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(35, 25, 0, 0);  // ����
//        }
//        else if (gray[4] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(35, 15, 0, 0);  // ǿ��
//        }
//        else if (gray[1] == GRAY_BLACK && gray[0] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(25, 35, 0, 0);  // ����
//        }
//        else if (gray[0] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(15, 35, 0, 0);  // ǿ��
//        }
//        else if (gray[0] == GRAY_BLACK && gray[1] == GRAY_BLACK &&
//                 gray[2] == GRAY_BLACK && gray[3] == GRAY_BLACK &&
//                 gray[4] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(30, 30, 0, 0);  // ʮ��·��ֱ��
//        }
//        else
//        {
//            Motor_SetSpeed_Async(10, 10, 0, 0);  // ģ��ֱ��
//        }

//        // ���汾�λҶ�״̬
//        for (int i = 0; i < 5; i++)
//        {
//            last_gray[i] = gray[i];
//        }
//    }
//    else
//    {
//        // --- ���ߴ���������һ��״̬ ---
//        if (last_gray[2] == GRAY_BLACK)
//        {
//            // ����ʶ���м�����⵽�ڣ�������в���
//            Motor_SetSpeed_Async(15, 35, 0, 0);  // ����ת �˴���Ϊ��������
//        }
//        else if (last_gray[0] == GRAY_BLACK || last_gray[1] == GRAY_BLACK)
//        {
//            // ԭ������࣬˵����תû��ɣ�������ת
//            Motor_SetSpeed_Async(15, 35, 0, 0);
//        }
//        else if (last_gray[3] == GRAY_BLACK || last_gray[4] == GRAY_BLACK)
//        {
//            // ԭ�����Ҳ࣬˵����תû��ɣ�������ת
//            Motor_SetSpeed_Async(35, 15, 0, 0);
//        }
//        else
//        {
//            // �������ߣ��޷��жϣ�ͣ����С������
//            Motor_SetSpeed_Async(0, 0, 0, 0);
//        }
//    }
//}




void SetTargetSpeed(int8_t left, int8_t right)
{
    target_left_speed = left;
    target_right_speed = right;
}



float PID_Update(PID_Controller* pid, float current) {
  float error = pid->target - current;
  
  // �����ֱ���
  if(fabs(error) < 5.0f) {  // ������Ż���
    pid->integral += error;
  }
  
  // ΢����
  float derivative = error - pid->last_error;
  
  // �������
  float output = pid->Kp * error 
               + pid->Ki * pid->integral 
               + pid->Kd * derivative;
  
  // ��¼�ϴ����
  pid->last_error = error;
  
  return output;
}


/* ������ */
float Calculate_Error(void) {
  float sum = 0, weightedSum = 0;
  
  for(int i=0; i<NUM_SENSORS; i++) {
    float val = gray[i] ;  // ��һ����0-1
    weightedSum += val * weights[i];
    sum += val;
  }
  
  return (sum > 0.1f) ? (weightedSum / sum) : 0;
}


/* ������� */
void Motor_Control(float output) {
	
  int left = baseSpeed + output + distance_output;
  int right = baseSpeed - output + distance_output;
  
  // ����PWM��Χ
  left = (left < 0) ? 0 : (left > 60) ? 60 : left;
  right = (right < 0) ? 0 : (right > 60) ? 60 : right;
	Motor_SetSpeed_Async(left,right,0,0);

}

float Distance_PID_Update(PID_Controller* pid, float current_mm)
{
    float error = current_mm - pid->target;

    if (fabs(error) < 5.0f)  // ������� 5cm ʱ�������
        pid->integral += error;

    float derivative = error - pid->last_error;
    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    pid->last_error = error;

    return output;  // ���Ϊ�ٶ���������������
}
