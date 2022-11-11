/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
//转换系数
#define ADC_CONVERT_SCALE    4096.0f
//#define ADC_CONVERT_REF_VOLT 3000.0f								//以mV为单位
#define ADC_CONVERT_REF_VOLT 3.0f								//以mV为单位
#define ADC_CONVERT_K				 ADC_CONVERT_REF_VOLT/ADC_CONVERT_SCALE
volatile uint16_t ADC_ConvertedValue[2];

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_REGULAR_RANK_2;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */
		HAL_ADCEx_Calibration_Start(&hadc1);					 //校准ADC

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

uint16_t Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //采样时间               
    HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //通道配置
	
    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	//返回最近一次ADC1规则组的转换结果
}

#define ADC_FILTERVALUE_MAX_CNT 20

static uint16_t ADC_FilterValue[2][ADC_FILTERVALUE_MAX_CNT];
static uint16_t ADC_FilterVlaue_Cnt[2];

//滚动存储
void hal_ADC_M4_Filter_Cycle_Save(uint8_t chn)
{
	uint16_t i;
	uint16_t tmp_code;
	uint16_t tmp_cnt;
	ADC_FilterVlaue_Cnt[chn]++;
	if(ADC_FilterVlaue_Cnt[chn]>ADC_FILTERVALUE_MAX_CNT){
		ADC_FilterVlaue_Cnt[chn]=ADC_FILTERVALUE_MAX_CNT;	
	}
	tmp_code = ADC_ConvertedValue[chn];
	tmp_cnt  = ADC_FilterVlaue_Cnt[chn];
	//滚动存储
	if(tmp_cnt<ADC_FILTERVALUE_MAX_CNT)	{
			ADC_FilterValue[chn][tmp_cnt]=tmp_code;
	}
	else{
		for(i=0;i<ADC_FILTERVALUE_MAX_CNT-1;i++){
			ADC_FilterValue[chn][i]=ADC_FilterValue[chn][i+1];
		}
		ADC_FilterValue[chn][ADC_FILTERVALUE_MAX_CNT-1] = tmp_code;
	}
}

//滤波处理
uint16_t hal_ADC_M4_Filter_handle(uint8_t chn)
{
	uint16_t tmp_rtn;
	uint16_t tmp_cnt;
	uint16_t i;
	uint32_t tmp_val;
	uint16_t tmp_code_dat[ADC_FILTERVALUE_MAX_CNT];

	//滑动存储
	hal_ADC_M4_Filter_Cycle_Save(chn);

	//滤波计算
	tmp_val = 0;
	tmp_rtn = 0;
	tmp_cnt = ADC_FilterVlaue_Cnt[chn];
	//缓存切换
	for(i=0;i<ADC_FILTERVALUE_MAX_CNT;i++){
		tmp_code_dat[i] = ADC_FilterValue[chn][i];
	}

	//采用滑动平均值方式滤波
	for(i=0;i<tmp_cnt;i++){
		tmp_val+=tmp_code_dat[i];
	}
	if(tmp_cnt!= 0){
		tmp_rtn = tmp_val/tmp_cnt;
	}
	return tmp_rtn;
}

//得到通道采集电压值
float hal_ADC_M4_get_value(uint8_t chn)
{
	float v;
	if(chn == 0)
	{
		ADC_ConvertedValue[chn]=Get_Adc(ADC_CHANNEL_11);
	}
	if(chn == 1)
	{
		ADC_ConvertedValue[chn]=Get_Adc(ADC_CHANNEL_10);
	}
	v = ADC_ConvertedValue[chn];
//	v = (float)hal_ADC_M4_Filter_handle(chn);
	v*= ADC_CONVERT_K;
	return v;
}


/* USER CODE END 1 */
