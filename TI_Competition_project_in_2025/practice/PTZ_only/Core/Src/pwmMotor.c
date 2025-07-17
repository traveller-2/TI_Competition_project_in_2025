//#include "pwmMotor.h"
//#include "tim.h"
//#include "gpio.h"
//#include "math.h"
//#include "encoder.h"
//#include "cmsis_os.h"


//// 自定义参数（需与你 CubeMX 设置对应）
//#define MOTOR1_TIMER       htim2
//#define MOTOR1_CHANNEL     TIM_CHANNEL_1
//#define MOTOR1_DIR_GPIO    GPIOC
//#define MOTOR1_DIR_PIN     GPIO_PIN_3

//#define MOTOR2_TIMER       htim2
//#define MOTOR2_CHANNEL     TIM_CHANNEL_3
//#define MOTOR2_DIR_GPIO    GPIOE
//#define MOTOR2_DIR_PIN     GPIO_PIN_15

//#define SAMPLE_TIME_SEC 0.01f  // 10ms
//#define MOTOR_MAX_PWM      1000

//int oled_rpm [2];
//int err[2];

//typedef struct {
//    float target_rpm;
//    float current_rpm;
//    float kp, ki, kd;
//    float prev_error;
//    float integral;
//} MotorPID;
////速度环PID三个参数在这里调,三个浮点数
//MotorPID motor1_pid = {0, 0, 0.8f, 0.05f, 0.02f, 0, 0};
//MotorPID motor2_pid = {0, 0, 0.8f, 0.05f, 0.02f, 0, 0};

//osMessageQueueId_t motorQueueHandle;

//void MotorControlTask(void *argument)
//{
//    Motor_Init();
//		Encoder_Init();

//    // 创建消息队列：长度为 10，每个元素是一个 MotorCmd_t
//    motorQueueHandle = osMessageQueueNew(10, sizeof(MotorCmd_t), NULL);

//    MotorCmd_t recv_cmd;
//		Motor_SetSpeedAsync(MOTOR1, 100);
//		Motor_SetSpeedAsync(MOTOR2, 100);
//	
//    while (1)
//    {
//        // 检查是否有新消息（非阻塞读取）
//        if (osMessageQueueGet(motorQueueHandle, &recv_cmd, NULL, 0) == osOK)
//        {
//            // 收到更新指令，设置目标转速
//            Motor_SetTargetRPM(recv_cmd.motor_id, recv_cmd.target_rpm);
//        }

//        Motor_PIDUpdate();  // 每10ms进行一次速度闭环
//        vTaskDelay(pdMS_TO_TICKS(10));
//    }
//}

///*************************************************************
//	*在主控程序或其他任务中调用：
//	*Motor_SetSpeedAsync(MOTOR1, 200);  // 改变左轮目标转速
//	*在 MotorControlTask 中接收并更新电机转速 → 自动生效
//	************************************************************/
//void Motor_SetSpeedAsync(uint8_t motor_id, float rpm)
//{
//    MotorCmd_t cmd = {motor_id, rpm};
//    osMessageQueuePut(motorQueueHandle, &cmd, 0, 0);  // 立即投递
//}


//// 初始化两个电机
//void Motor_Init(void)
//{
//    HAL_TIM_PWM_Start(&MOTOR1_TIMER, MOTOR1_CHANNEL);
//    HAL_TIM_PWM_Start(&MOTOR2_TIMER, MOTOR2_CHANNEL);

//    HAL_GPIO_WritePin(MOTOR1_DIR_GPIO, MOTOR1_DIR_PIN, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(MOTOR2_DIR_GPIO, MOTOR2_DIR_PIN, GPIO_PIN_SET);
//}

//// 设置方向和 PWM
//static void Motor_SetPWM(uint8_t id, int16_t speed)
//{
//    GPIO_TypeDef* dir_gpio;
//    uint16_t dir_pin;
//    TIM_HandleTypeDef* htim;
//    uint32_t channel;

//    if (id == MOTOR1) {
//        dir_gpio = MOTOR1_DIR_GPIO;
//        dir_pin = MOTOR1_DIR_PIN;
//        htim = &MOTOR1_TIMER;
//        channel = MOTOR1_CHANNEL;
//    } else {
//        dir_gpio = MOTOR2_DIR_GPIO;
//        dir_pin = MOTOR2_DIR_PIN;
//        htim = &MOTOR2_TIMER;
//        channel = MOTOR2_CHANNEL;
//    }

//    if (speed >= 0) {
//        HAL_GPIO_WritePin(dir_gpio, dir_pin, GPIO_PIN_SET);
//    } else {
//        HAL_GPIO_WritePin(dir_gpio, dir_pin, GPIO_PIN_RESET);
//        speed = -speed;
//    }

//    if (speed > MOTOR_MAX_PWM) speed = MOTOR_MAX_PWM;
//    __HAL_TIM_SET_COMPARE(htim, channel, speed);
//}

//// 设置目标转速
//void Motor_SetTargetRPM(uint8_t motor_id, float rpm)
//{
//    if (motor_id == MOTOR1)
//        motor1_pid.target_rpm = rpm;
//    else
//        motor2_pid.target_rpm = rpm;
//}

//// 获取当前转速（你需要补上真实测速逻辑）
//float Motor_GetCurrentRPM(uint8_t motor_id)
//{
//    return (motor_id == MOTOR1) ? motor1_pid.current_rpm : motor2_pid.current_rpm;
//}

//static float MeasureMotor1RPM(void)
//{
//    return Encoder_GetRPM(1);
//}

//static float MeasureMotor2RPM(void)
//{
//    return Encoder_GetRPM(2);
//}


//// PID更新函数
//#define SAMPLE_TIME_SEC 0.01f  // 10ms

//void Motor_PIDUpdate(void)
//{
//    // 更新当前转速
//    motor1_pid.current_rpm = MeasureMotor1RPM();
//    oled_rpm[0] = motor1_pid.current_rpm;

//    motor2_pid.current_rpm = MeasureMotor2RPM();
//    oled_rpm[1] = motor2_pid.current_rpm;

//    // ------------------- PID for MOTOR1 -------------------
//    float error1 = motor1_pid.target_rpm - motor1_pid.current_rpm;
//    motor1_pid.integral += error1 * SAMPLE_TIME_SEC;

//    // 限制积分项防止饱和
//    if (motor1_pid.integral > 1000) motor1_pid.integral = 1000;
//    if (motor1_pid.integral < -1000) motor1_pid.integral = -1000;

//    err[0] = motor1_pid.integral;

//    float d1 = error1 - motor1_pid.prev_error;
//    motor1_pid.prev_error = error1;

//    int16_t output1 = motor1_pid.kp * error1
//                    + motor1_pid.ki * motor1_pid.integral
//                    + motor1_pid.kd * d1;
//    Motor_SetPWM(MOTOR1, output1);

//    // ------------------- PID for MOTOR2 -------------------
//    float error2 = motor2_pid.target_rpm - motor2_pid.current_rpm;
//    motor2_pid.integral += error2 * SAMPLE_TIME_SEC;

//    if (motor2_pid.integral > 1000) motor2_pid.integral = 1000;
//    if (motor2_pid.integral < -1000) motor2_pid.integral = -1000;

//    err[1] = motor2_pid.integral;

//    float d2 = error2 - motor2_pid.prev_error;
//    motor2_pid.prev_error = error2;

//    int16_t output2 = motor2_pid.kp * error2
//                    + motor2_pid.ki * motor2_pid.integral
//                    + motor2_pid.kd * d2;
//    Motor_SetPWM(MOTOR2, output2);
//}

