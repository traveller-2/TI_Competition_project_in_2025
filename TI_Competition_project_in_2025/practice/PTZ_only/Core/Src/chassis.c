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

// 初始化位置 PID 控制器
PID_Controller posPID = {
		.target = 0,
		.Kp = 8.5f,
		.Ki = 0.0f,
		.Kd = -0.83f
};

PID_Controller distancePID = {
    .target = 35.0f,   // 距离目标，例如 20cm
    .Kp = 5.5f,        // 可调：响应速度
    .Ki = 0.0f,       // 可调：长期偏移
    .Kd = 0.0f         // 可调：防抖动
};

//PID_Controller yaw_pid = {
//    .target = 0,
//    .Kp = 1.0f,
//    .Ki = 0.0f,
//    .Kd = 0.0f
//};

// FreeRTOS 任务句柄
osThreadId_t chassisTaskHandle;

// GPIO端口和引脚定义（CubeMX配置）
#define GRAY_GPIO_PORT_L_M   	GPIOF
#define GRAY_LEFT2_PIN   			GPIO_PIN_13
#define GRAY_LEFT1_PIN   			GPIO_PIN_14
#define GRAY_MID_PIN     			GPIO_PIN_15
#define GRAY_GPIO_PORT_R   		GPIOG
#define GRAY_RIGHT1_PIN  			GPIO_PIN_0
#define GRAY_RIGHT2_PIN  			GPIO_PIN_1


extern int flag0;

int flag = 0;
static uint8_t last_valid_gray[5] = {0};  // 保存上一次有效状态

//加速度控制
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
//  float target;      // 目标值
//  float current;     // 当前值
//  float Kp, Ki, Kd; // PID参数
//  float integral;    // 积分项
//  float last_error;  // 上次误差
//} PID_Controller;



// FreeRTOS 底盘循迹任务
void ChassisFollowTask(void *argument)
{

    Motor_SetSpeed_Async(40, 40, 0, 0);
    osDelay(300);

    while (1)
    {		
				
        // 获取姿态角（pitch）
        imu901_attitude_t att = imu901_get_attitude();
        float pitch = -(att.roll - 7.7f);

        // 超声波避障：目标距离维持 28cm，如果太远则不控制
        if (hcsr04_distance_mm < 35)
        {
            distance_output = Distance_PID_Update(&distancePID, (float)hcsr04_distance_mm);
        }
        else
        {
            distance_output = 0.0f;
        }

        // 读取灰度传感器并计算循迹误差
        Read_All_GraySensors(gray);
        posError = Calculate_Error();
        float turn_output = PID_Update(&posPID, posError);

        // 执行底盘控制
        Motor_Control(turn_output);

        // 循迹周期：10ms
        osDelay(10);

        // 判断是否到达停止条件：
        // - 左/右侧全部偏离（可能是终点）
        // - 地面无线 && 当前非上坡（pitch < 5°）

        if(((gray[0]==0&&gray[1]==0&&gray[2]==0&&gray[3]==0)||	(gray[4]==0&&gray[1]==0&&gray[2]==0&&gray[3]==0))&&( fabs(pitch) <= 5)&&stop_flag == 1)
        {
            Motor_SetSpeed_Async(0, 0, 0, 0);  // 停止底盘
            osDelay(200);  // 稍作延时稳定
            flag = 1;      // 进入下一个阶段的标志
            break;         // 退出任务（或让任务结束）
        }
				
//				//超车任务
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
////					float target_yaw = att.yaw - 45.0f;       // 想要的角度（目标朝向）
////					yaw_pid.target = 0;  											// 目标是误差为0
////					uint32_t start = HAL_GetTick();
////					while (HAL_GetTick() - start < 2000)//控制循环运行时间
////					{
////							imu901_attitude_t att = imu901_get_attitude();
////							float yaw_now = att.yaw;

////							// 计算角度误差（处理角度绕回问题）
////							float yaw_error = target_yaw - yaw_now;
////							if (yaw_error > 180.0f) yaw_error -= 360.0f;
////							if (yaw_error < -180.0f) yaw_error += 360.0f;

////							// 更新 PID 输出：转向控制量（左右轮差速）
////							float turn_output = PID_Update(&yaw_pid, yaw_error);

////							// 计算左右轮速度（差速控制）
////							float left_speed = baseSpeed + turn_output;
////							float right_speed = baseSpeed - turn_output;
////							
////						  left_speed = (left_speed < 0) ? 0 : (left_speed > 60) ? 60 : left_speed;
////							right_speed = (right_speed < 0) ? 0 : (right_speed > 60) ? 60 : right_speed;
////							// 发送速度指令
////							Motor_SetSpeed_Async(left_speed, right_speed, 0, 0);

////							osDelay(20);
////					}
////					
////					target_yaw = att.yaw + 75.0f;
////					while(1)
////					{
////							imu901_attitude_t att = imu901_get_attitude();
////							float yaw_now = att.yaw;

////							// 计算角度误差（处理角度绕回问题）
////							float yaw_error = target_yaw - yaw_now;
////							if (yaw_error > 180.0f) yaw_error -= 360.0f;
////							if (yaw_error < -180.0f) yaw_error += 360.0f;

////							// 更新 PID 输出：转向控制量（左右轮差速）
////							float turn_output = PID_Update(&yaw_pid, yaw_error);

////							// 计算左右轮速度（差速控制）
////							float left_speed = baseSpeed + turn_output;
////							float right_speed = baseSpeed - turn_output;
////							
////						  left_speed = (left_speed < 0) ? 0 : (left_speed > 60) ? 60 : left_speed;
////							right_speed = (right_speed < 0) ? 0 : (right_speed > 60) ? 60 : right_speed;
////							// 发送速度指令
////							Motor_SetSpeed_Async(left_speed, right_speed, 0, 0);
////							
////							if(gray[2]==0||gray[3]==0||gray[4]==0)
////							{
////								break;
////							}
////							osDelay(20);
////					}
//				}注释标记：超车到这里结束
				
    }		
		vTaskDelete(NULL);  // 删除当前任务
}

		
/***********************************说明*********************************************
	*Servo_Init();                    // 第一步：初始化
	*Servo_SetID(0, 1);               // 第二步（初次）：修改ID（每个舵机唯一）
	*Servo_SetMode(1, 1);             // 第三步：设置舵机模式（如270度）
	*
	* 控制多个舵机
	*Servo_QueueControl(1, 1500, 1000);
	*Servo_QueueControl(2, 2000, 1000);
	*
	* 可选：读取舵机角度
	*char buffer[20];
	*Servo_ReadAngle(1, buffer, 100);
	************************************************************************************/

// 从左到右依次读取灰度值
static void Read_All_GraySensors(uint8_t *gray)
{
    static uint8_t last_valid_gray[5] = {0};
    static uint8_t last_pattern = 0;

    // 当前读取
    uint8_t raw[5];
    raw[0] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_LEFT2_PIN);   // 左2
    raw[1] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_LEFT1_PIN);   // 左1
    raw[2] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_L_M, GRAY_MID_PIN);     // 中
    raw[3] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_R, GRAY_RIGHT1_PIN);    // 右1
    raw[4] = !HAL_GPIO_ReadPin(GRAY_GPIO_PORT_R, GRAY_RIGHT2_PIN);    // 右2

    // 检查是否全为白（全部为1）
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
        // 判断上一次是否是“仅一侧单独为0”
        //uint8_t sum = last_valid_gray[0] + last_valid_gray[1] + last_valid_gray[2] + last_valid_gray[3] + last_valid_gray[4];
        if (last_valid_gray[0] == 0 || last_valid_gray[4] == 0)
        {
            // 如果上次只有一个0，保持不变
            for (int i = 0; i < 5; i++)
            {
                gray[i] = last_valid_gray[i];
            }
            return;
        }
    }

    // 否则，正常更新 gray
    for (int i = 0; i < 5; i++)
    {
        gray[i] = raw[i];
        last_valid_gray[i] = raw[i];  // 保存为最近一次有效灰度值
    }
}


//// 控制逻辑（两轮差速）static void FollowLine(const uint8_t *gray)
//static void FollowLine(const uint8_t *gray)
//{
//    static uint8_t last_gray[5] = {0};  // 上一次灰度状态

//    // 状态保存标志位
//    uint8_t detected = 0;

//    // 检测是否有任何一个传感器识别到黑线
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
//        // 正常循迹处理
//        if (gray[2] == GRAY_BLACK && gray[1] == GRAY_WHITE && gray[3] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(30, 30, 0, 0);
//        }
//        else if (gray[3] == GRAY_BLACK && gray[4] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(35, 25, 0, 0);  // 向右
//        }
//        else if (gray[4] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(35, 15, 0, 0);  // 强右
//        }
//        else if (gray[1] == GRAY_BLACK && gray[0] == GRAY_WHITE)
//        {
//            Motor_SetSpeed_Async(25, 35, 0, 0);  // 向左
//        }
//        else if (gray[0] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(15, 35, 0, 0);  // 强左
//        }
//        else if (gray[0] == GRAY_BLACK && gray[1] == GRAY_BLACK &&
//                 gray[2] == GRAY_BLACK && gray[3] == GRAY_BLACK &&
//                 gray[4] == GRAY_BLACK)
//        {
//            Motor_SetSpeed_Async(30, 30, 0, 0);  // 十字路口直行
//        }
//        else
//        {
//            Motor_SetSpeed_Async(10, 10, 0, 0);  // 模糊直行
//        }

//        // 保存本次灰度状态
//        for (int i = 0; i < 5; i++)
//        {
//            last_gray[i] = gray[i];
//        }
//    }
//    else
//    {
//        // --- 脱线处理，根据上一次状态 ---
//        if (last_gray[2] == GRAY_BLACK)
//        {
//            // 虚线识别，中间曾检测到黑，左侧绕行策略
//            Motor_SetSpeed_Async(15, 35, 0, 0);  // 先左转 此处即为超车部分
//        }
//        else if (last_gray[0] == GRAY_BLACK || last_gray[1] == GRAY_BLACK)
//        {
//            // 原本在左侧，说明左转没完成，继续右转
//            Motor_SetSpeed_Async(15, 35, 0, 0);
//        }
//        else if (last_gray[3] == GRAY_BLACK || last_gray[4] == GRAY_BLACK)
//        {
//            // 原本在右侧，说明左转没完成，继续左转
//            Motor_SetSpeed_Async(35, 15, 0, 0);
//        }
//        else
//        {
//            // 彻底脱线，无法判断，停车或小幅搜索
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
  
  // 抗积分饱和
  if(fabs(error) < 5.0f) {  // 死区外才积分
    pid->integral += error;
  }
  
  // 微分项
  float derivative = error - pid->last_error;
  
  // 计算输出
  float output = pid->Kp * error 
               + pid->Ki * pid->integral 
               + pid->Kd * derivative;
  
  // 记录上次误差
  pid->last_error = error;
  
  return output;
}


/* 误差计算 */
float Calculate_Error(void) {
  float sum = 0, weightedSum = 0;
  
  for(int i=0; i<NUM_SENSORS; i++) {
    float val = gray[i] ;  // 归一化到0-1
    weightedSum += val * weights[i];
    sum += val;
  }
  
  return (sum > 0.1f) ? (weightedSum / sum) : 0;
}


/* 电机控制 */
void Motor_Control(float output) {
	
  int left = baseSpeed + output + distance_output;
  int right = baseSpeed - output + distance_output;
  
  // 限制PWM范围
  left = (left < 0) ? 0 : (left > 60) ? 60 : left;
  right = (right < 0) ? 0 : (right > 60) ? 60 : right;
	Motor_SetSpeed_Async(left,right,0,0);

}

float Distance_PID_Update(PID_Controller* pid, float current_mm)
{
    float error = current_mm - pid->target;

    if (fabs(error) < 5.0f)  // 距离大于 5cm 时允许积分
        pid->integral += error;

    float derivative = error - pid->last_error;
    float output = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    pid->last_error = error;

    return output;  // 输出为速度增量（可正负）
}
