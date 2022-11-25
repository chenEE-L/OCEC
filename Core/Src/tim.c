/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "FreeRTOS.h"
#include "semphr.h"

/* USER CODE BEGIN 0 */
extern SemaphoreHandle_t singleDateDealSemaphore;	//二值信号量句柄

/* USER CODE END 0 */

TIM_HandleTypeDef htim4;

/* TIM4 init function */
void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 7200-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 2000-1;                  //72M/7200=10K的计数频率，自动重装载为1000，那么PWM频率为10000/1000=10
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
//	HAL_TIM_Base_Start_IT(&htim4); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE   

  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1000;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
    HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_2);//开启PWM通道2

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);
	

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* TIM4 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE BEGIN TIM4_MspInit 1 */
    HAL_NVIC_SetPriority(TIM4_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
  /* USER CODE END TIM4_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspPostInit 0 */

  /* USER CODE END TIM4_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM4 GPIO Configuration
    PB7     ------> TIM4_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM4_MspPostInit 1 */

  /* USER CODE END TIM4_MspPostInit 1 */
  }

}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim4);
}


/***********************************************************************************/

TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
TIM_OC_InitTypeDef 	TIM3_CH2Handler;	//定时器3通道2句柄
TIM_OC_InitTypeDef 	TIM3_CH3Handler;	//定时器3通道3句柄

static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);


void TIM3_Init()
{  
	
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=72-1;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=0xFFFE;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
//		HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
//	  HAL_TIM_Base_Init(&TIM3_Handler);
		HAL_TIM_OC_Init(&TIM3_Handler);

    TIM3_CH2Handler.OCMode=TIM_OCMODE_TIMING; //
		HAL_TIM_OC_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);
//		HAL_TIM_OC_Start(&TIM3_Handler,TIM_CHANNEL_2);
//		HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_2);

	  TIM3_CH3Handler.OCMode=TIM_OCMODE_TIMING; //
		HAL_TIM_OC_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);
//		HAL_TIM_OC_Start(&TIM3_Handler,TIM_CHANNEL_3);
//		HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_3);

}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,5,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}

}


// us级定时器 目前用于MODBUS判断帧结束,内加补偿

void TIM_SetTIM3Compare2(uint32_t compare)
{
	TIM3->CCR2=compare; 
}
void TIM_SetTIM3Compare3(uint32_t compare)
{
	TIM3->CCR3=compare; 
}


void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        执行下面这个语句，时长 = 18us (通过逻辑分析仪测量IO翻转)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = __HAL_TIM_GetCounter(&TIM3_Handler);    	/* 读取当前的计数器值 */
    cnt_tar = cnt_now + _uiTimeOut;			/* 计算捕获的计数器值 */
    if (_CC == 2)
    {
        s_TIM_CallBack2 = (void (*)(void))_pCallBack;
				TIM_SetTIM3Compare2(cnt_tar);	//修改比较值，修改占空比
				HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_2);
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;
				TIM_SetTIM3Compare3(cnt_tar);	//修改比较值，修改占空比
				HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_3);
    }
		else
    {
        return;
    }
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{

    HAL_TIM_IRQHandler(&TIM3_Handler);
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{  

		if(htim->Instance==TIM3)
    {
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
        {
					 HAL_TIM_OC_Stop_IT(&TIM3_Handler, TIM_CHANNEL_2); 	/* 禁能中断 */

		/* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
					 s_TIM_CallBack2();
        }
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
        {
					 HAL_TIM_OC_Stop_IT(&TIM3_Handler, TIM_CHANNEL_3); 	/* 禁能中断 */

		/* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
					 s_TIM_CallBack3();
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
	BaseType_t xHigherPriorityTaskWoken;

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

	if (htim->Instance == TIM4) {
		
		xSemaphoreGiveFromISR(singleDateDealSemaphore,&xHigherPriorityTaskWoken);	//释放二值信号量
  }

  /* USER CODE END Callback 1 */
}


/* USER CODE END 1 */
