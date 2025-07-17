//#include "encoder.h"
//#include "tim.h"

//// ------------------ 编码器参数 ------------------
//#define ENCODER_PPR      (13 * 4)    // 编码器本体每圈脉冲数 × 四倍频
//#define GEAR_RATIO       30          // 电机减速比
//#define SAMPLE_MS        10          // 取样间隔(ms)

//// ------------------ TIM 宏定义（左轮/右轮） ------------------
//#define ENCODER_L_TIM         htim3
//#define ENCODER_L_GET_CNT()   __HAL_TIM_GET_COUNTER(&ENCODER_L_TIM)
//#define ENCODER_L_RESET_CNT() __HAL_TIM_SET_COUNTER(&ENCODER_L_TIM, 0)

//#define ENCODER_R_TIM         htim4
//#define ENCODER_R_GET_CNT()   __HAL_TIM_GET_COUNTER(&ENCODER_R_TIM)
//#define ENCODER_R_RESET_CNT() __HAL_TIM_SET_COUNTER(&ENCODER_R_TIM, 0)

//// ------------------ 全局变量 ------------------
//static int16_t last_cnt_l = 0;
//static int16_t last_cnt_r = 0;
//float rpm ;


//// ------------------ 初始化 ------------------
//void Encoder_Init(void)
//{
//    HAL_TIM_Encoder_Start(&ENCODER_L_TIM, TIM_CHANNEL_ALL);
//    HAL_TIM_Encoder_Start(&ENCODER_R_TIM, TIM_CHANNEL_ALL);

//    ENCODER_L_RESET_CNT();
//    ENCODER_R_RESET_CNT();

//    last_cnt_l = 0;
//    last_cnt_r = 0;
//}

//// ------------------ 获取增量 ------------------
//int16_t Encoder_GetDelta(uint8_t id)
//{
//    int16_t now, delta;
//    int16_t *last_cnt;

//    if (id == 1) {
//        now = ENCODER_L_GET_CNT();
//        last_cnt = &last_cnt_l;
//    } else {
//        now = ENCODER_R_GET_CNT();
//        last_cnt = &last_cnt_r;
//    }

//    delta = now - *last_cnt;

//    // 处理编码器溢出（TIM为16位）
//    if (delta > 32767) delta -= 65536;
//    if (delta < -32768) delta += 65536;

//    *last_cnt = now;
//    return delta;
//}

//// ------------------ 获取 RPM ------------------
//float Encoder_GetRPM(uint8_t id)
//{
//    int16_t delta = Encoder_GetDelta(id);

//    // 每 SAMPLE_MS 毫秒采样，转换为分钟
//    rpm = (delta * 6000.0f) / (ENCODER_PPR * GEAR_RATIO);  // 10ms → 6000

//    return rpm;
//}
