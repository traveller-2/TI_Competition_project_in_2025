/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_SDA_Pin GPIO_PIN_0
#define OLED_SDA_GPIO_Port GPIOF
#define OLED_SCL_Pin GPIO_PIN_1
#define OLED_SCL_GPIO_Port GPIOF
#define MOT_L_DIR_Pin GPIO_PIN_3
#define MOT_L_DIR_GPIO_Port GPIOC
#define MOT_L_PWM_Pin GPIO_PIN_0
#define MOT_L_PWM_GPIO_Port GPIOA
#define SERVO_PWM_Pin GPIO_PIN_1
#define SERVO_PWM_GPIO_Port GPIOA
#define MOT_L_ENA_Pin GPIO_PIN_6
#define MOT_L_ENA_GPIO_Port GPIOA
#define MOT_L_ENB_Pin GPIO_PIN_7
#define MOT_L_ENB_GPIO_Port GPIOA
#define TRACK_L2_Pin GPIO_PIN_13
#define TRACK_L2_GPIO_Port GPIOF
#define TRACK_L1_Pin GPIO_PIN_14
#define TRACK_L1_GPIO_Port GPIOF
#define TRACK_M_Pin GPIO_PIN_15
#define TRACK_M_GPIO_Port GPIOF
#define TRACK_R1_Pin GPIO_PIN_0
#define TRACK_R1_GPIO_Port GPIOG
#define TRACK_R2_Pin GPIO_PIN_1
#define TRACK_R2_GPIO_Port GPIOG
#define TRIG_Pin GPIO_PIN_12
#define TRIG_GPIO_Port GPIOE
#define ECHO_Pin GPIO_PIN_13
#define ECHO_GPIO_Port GPIOE
#define MOT_R_DIR_Pin GPIO_PIN_15
#define MOT_R_DIR_GPIO_Port GPIOE
#define MOT_R_PWM_Pin GPIO_PIN_10
#define MOT_R_PWM_GPIO_Port GPIOB
#define IMU_TX_Pin GPIO_PIN_8
#define IMU_TX_GPIO_Port GPIOD
#define IMU_RX_Pin GPIO_PIN_9
#define IMU_RX_GPIO_Port GPIOD
#define MOT_R_ENA_Pin GPIO_PIN_12
#define MOT_R_ENA_GPIO_Port GPIOD
#define MOT_R_ENB_Pin GPIO_PIN_13
#define MOT_R_ENB_GPIO_Port GPIOD
#define BLU_TX_Pin GPIO_PIN_9
#define BLU_TX_GPIO_Port GPIOA
#define BLU_RX_Pin GPIO_PIN_10
#define BLU_RX_GPIO_Port GPIOA
#define SERVO_TX_Pin GPIO_PIN_5
#define SERVO_TX_GPIO_Port GPIOD
#define SERVO_RX_Pin GPIO_PIN_6
#define SERVO_RX_GPIO_Port GPIOD
#define Xshut_Pin GPIO_PIN_5
#define Xshut_GPIO_Port GPIOB
#define LASER_SCL_Pin GPIO_PIN_6
#define LASER_SCL_GPIO_Port GPIOB
#define LASER_SDA_Pin GPIO_PIN_7
#define LASER_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
