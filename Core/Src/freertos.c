/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "laser.h"
#include "flow.h"
#include "analog.h"
#include "mods.h"
#include "iwdt.h"
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
osThreadId laserControlHandle;
osThreadId flowCollectHandle;
osThreadId analogCollectHandle;
osThreadId modsUpdateHandle;
osThreadId flowSetHandle;
osThreadId iwdtHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId logicControHandle;

void logicControTask(void const * argument);

/* USER CODE END FunctionPrototypes */

void laserControlTask(void const * argument);
void flowCollectTask(void const * argument);
void analogCollectTask(void const * argument);
void modsUpdateTask(void const * argument);
void flowSetTask(void const * argument);
void iwdtTask(void const * argument);

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
  /* definition and creation of laserControl */
  osThreadDef(laserControl, laserControlTask, osPriorityNormal, 0, 128);
  laserControlHandle = osThreadCreate(osThread(laserControl), NULL);

  /* definition and creation of flowCollect */
  osThreadDef(flowCollect, flowCollectTask, osPriorityBelowNormal, 0, 256);
  flowCollectHandle = osThreadCreate(osThread(flowCollect), NULL);

  /* definition and creation of analogCollect */
  osThreadDef(analogCollect, analogCollectTask, osPriorityNormal, 0, 128);
  analogCollectHandle = osThreadCreate(osThread(analogCollect), NULL);

  /* definition and creation of modsUpdate */
  osThreadDef(modsUpdate, modsUpdateTask, osPriorityNormal, 0, 128);
  modsUpdateHandle = osThreadCreate(osThread(modsUpdate), NULL);

  /* definition and creation of flowSet */
  osThreadDef(flowSet, flowSetTask, osPriorityNormal, 0, 128);
  flowSetHandle = osThreadCreate(osThread(flowSet), NULL);

  /* definition and creation of iwdt */
  osThreadDef(iwdt, iwdtTask, osPriorityNormal, 0, 128);
  iwdtHandle = osThreadCreate(osThread(iwdt), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(logicContro, logicControTask, osPriorityBelowNormal, 0, 128);
  logicControHandle = osThreadCreate(osThread(logicContro), NULL);
  /* USER CODE END RTOS_THREADS */

}


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

