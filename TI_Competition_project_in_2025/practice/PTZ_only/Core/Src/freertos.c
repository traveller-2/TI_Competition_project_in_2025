/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Servo_USART */
osThreadId_t Servo_USARTHandle;
const osThreadAttr_t Servo_USART_attributes = {
  .name = "Servo_USART",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Rx_Task */
osThreadId_t Rx_TaskHandle;
const osThreadAttr_t Rx_Task_attributes = {
  .name = "Rx_Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for PTZ */
osThreadId_t PTZHandle;
const osThreadAttr_t PTZ_attributes = {
  .name = "PTZ",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for OLED */
osThreadId_t OLEDHandle;
const osThreadAttr_t OLED_attributes = {
  .name = "OLED",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void ServoControlTask(void *argument);
void BT_Comm_Task(void *argument);
void PTZ_TASK(void *argument);
void MotorIMU_Monitor_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Servo_USART */
  Servo_USARTHandle = osThreadNew(ServoControlTask, NULL, &Servo_USART_attributes);

  /* creation of Rx_Task */
  Rx_TaskHandle = osThreadNew(BT_Comm_Task, NULL, &Rx_Task_attributes);

  /* creation of PTZ */
  PTZHandle = osThreadNew(PTZ_TASK, NULL, &PTZ_attributes);

  /* creation of OLED */
  OLEDHandle = osThreadNew(MotorIMU_Monitor_Task, NULL, &OLED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_ServoControlTask */
/**
* @brief Function implementing the Servo_USART thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ServoControlTask */
__weak void ServoControlTask(void *argument)
{
  /* USER CODE BEGIN ServoControlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ServoControlTask */
}

/* USER CODE BEGIN Header_BT_Comm_Task */
/**
* @brief Function implementing the BT_Comm thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BT_Comm_Task */
__weak void BT_Comm_Task(void *argument)
{
  /* USER CODE BEGIN BT_Comm_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END BT_Comm_Task */
}

/* USER CODE BEGIN Header_PTZ_TASK */
/**
* @brief Function implementing the PTZ thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PTZ_TASK */
__weak void PTZ_TASK(void *argument)
{
  /* USER CODE BEGIN PTZ_TASK */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END PTZ_TASK */
}

/* USER CODE BEGIN Header_MotorIMU_Monitor_Task */
/**
* @brief Function implementing the OLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MotorIMU_Monitor_Task */
__weak void MotorIMU_Monitor_Task(void *argument)
{
  /* USER CODE BEGIN MotorIMU_Monitor_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END MotorIMU_Monitor_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

