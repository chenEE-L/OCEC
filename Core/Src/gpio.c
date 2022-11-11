/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC6 PC7 PC8
                           PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15
                           PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

void bsp_DoOn(uint8_t _no)
{
		_no--;

	if (_no == 0)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	}
	if (_no == 1)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	}
	if (_no == 2)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	}
	if (_no == 3)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	}
	if (_no == 4)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	}
	if (_no == 5)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	}
	if (_no == 6)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
	if (_no == 7)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
	}
	if (_no == 8)
	{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	}	
	if (_no == 9)
	{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	}	

}
void bsp_DoOff(uint8_t _no)
{
		_no--;

	if (_no == 0)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	}
	if (_no == 1)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	if (_no == 2)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	if (_no == 3)
	{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	}
	if (_no == 4)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	}
	if (_no == 5)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	if (_no == 6)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	if (_no == 7)
	{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	if (_no == 8)
	{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}	
	if (_no == 9)
	{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	}	

}

uint8_t bsp_IsDoOn(uint8_t _no)
{
	if (_no == 1)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 3)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 4)
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 5)
	{
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 6)
	{
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 7)
	{
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 8)
	{
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 9)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8))
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 10)
	{
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11))
		{
			return 1;
		}
		return 0;
	}
	

	return 0;
}

void bsp_DoOffAll()
{

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
}


/* USER CODE END 2 */
