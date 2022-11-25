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
extern SemaphoreHandle_t singleDateDealSemaphore;	//��ֵ�ź������

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
  htim4.Init.Period = 2000-1;                  //72M/7200=10K�ļ���Ƶ�ʣ��Զ���װ��Ϊ1000����ôPWMƵ��Ϊ10000/1000=10
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
//	HAL_TIM_Base_Start_IT(&htim4); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE   

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
    HAL_TIM_PWM_Start_IT(&htim4,TIM_CHANNEL_2);//����PWMͨ��2

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

TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
TIM_OC_InitTypeDef 	TIM3_CH2Handler;	//��ʱ��3ͨ��2���
TIM_OC_InitTypeDef 	TIM3_CH3Handler;	//��ʱ��3ͨ��3���

static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);


void TIM3_Init()
{  
	
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=72-1;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=0xFFFE;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
//		HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE
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

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,5,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}

}


// us����ʱ�� Ŀǰ����MODBUS�ж�֡����,�ڼӲ���

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
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת)
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

    cnt_now = __HAL_TIM_GetCounter(&TIM3_Handler);    	/* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;			/* ���㲶��ļ�����ֵ */
    if (_CC == 2)
    {
        s_TIM_CallBack2 = (void (*)(void))_pCallBack;
				TIM_SetTIM3Compare2(cnt_tar);	//�޸ıȽ�ֵ���޸�ռ�ձ�
				HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_2);
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;
				TIM_SetTIM3Compare3(cnt_tar);	//�޸ıȽ�ֵ���޸�ռ�ձ�
				HAL_TIM_OC_Start_IT(&TIM3_Handler, TIM_CHANNEL_3);
    }
		else
    {
        return;
    }
}

//��ʱ��3�жϷ�����
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
					 HAL_TIM_OC_Stop_IT(&TIM3_Handler, TIM_CHANNEL_2); 	/* �����ж� */

		/* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
					 s_TIM_CallBack2();
        }
        if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
        {
					 HAL_TIM_OC_Stop_IT(&TIM3_Handler, TIM_CHANNEL_3); 	/* �����ж� */

		/* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
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
		
		xSemaphoreGiveFromISR(singleDateDealSemaphore,&xHigherPriorityTaskWoken);	//�ͷŶ�ֵ�ź���
  }

  /* USER CODE END Callback 1 */
}


/* USER CODE END 1 */
