#include "mods.h"
#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "debug_assert.h"
/* USER CODE BEGIN Header_temCollectTask */
/**
  * @brief  Function implementing the temCollect thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_temCollectTask */





//TimerHandle_t	OneShotTimer_Handle;			//单次定时器句柄

void MODSInit(void)
{
//	OneShotTimer_Handle=xTimerCreate((const char*			)"OneShotTimer", 
//										 (TickType_t			)1,
//										 (UBaseType_t			)pdFALSE,
//										 (void*					)1,
//										 (TimerCallbackFunction_t)OneShotCallback); //单次定时器，周期1ms(1个时钟节拍)，单次模式	
//	ASSERT(OneShotTimer_Handle);


}





void modsUpdateTask(void const * argument)
{


	for(;;)
		{


			MODS_Poll();
			
			osDelay(5);
		}
		/* USER CODE END temCollectTask */
}

