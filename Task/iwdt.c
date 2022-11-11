#include "iwdt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
/* USER CODE BEGIN Header_temCollectTask */
/**
  * @brief  Function implementing the temCollect thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_temCollectTask */

static uint8_t wdt_state=0;
static uint8_t delay_reset=1;



static uint8_t wdog(void)
{
	if(wdt_state)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		wdt_state = 0;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		wdt_state = 1;
	}

	return wdt_state;
}


void exdog_reset(void)
{
	wdog();
	delay_reset=10;		//延长看门狗喂狗时间，会引起外部看门狗复位
}

void iwdtTask(void const * argument)
{
  /* USER CODE BEGIN temCollectTask */
  /* Infinite loop */
  for(;;)
  {
		uint8_t cur; 
		cur = wdog();
		osDelay((200L+600L*cur)*delay_reset);	//反相灯 0时间短，1时间长
  }
  /* USER CODE END temCollectTask */
}

