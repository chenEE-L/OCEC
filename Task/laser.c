#include "laser.h"
#include "debug_assert.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
/* USER CODE BEGIN Header_temCollectTask */
/**
  * @brief  Function implementing the temCollect thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_temCollectTask */


EventGroupHandle_t laserGroupHandler;	//�¼���־����

laser_para laserPWM;


void laser_init()
{
	 MX_TIM4_Init();// 10HZ ,50%
	
	laserGroupHandler=xEventGroupCreate();	
	ASSERT(laserGroupHandler);
	laserPWM.frequency = 5;
	laserPWM.duty =50;

}


void laserControlTask(void const * argument)
{
	uint16_t f,d;
	EventBits_t EventValue = 0;

  /* USER CODE BEGIN temCollectTask */


  /* Infinite loop */
   for(;;)
  {
		if(laserGroupHandler!=NULL)
		{

				EventValue=xEventGroupGetBits(laserGroupHandler);	//��ȡ�¼����
				switch((uint8_t)EventValue)
				{
					case laser_Frequency_Bit:
						f = laserPWM.frequency;
						f = 10000/f;
						__HAL_TIM_SET_AUTORELOAD(&htim4, f-1);  //������װ��ֵ
						xEventGroupClearBits(laserGroupHandler,laser_Frequency_Bit);
						xEventGroupSetBits(laserGroupHandler,laser_Duty_Bit);//������װ��ֵ����Ҫ��������ռ�ձ�

						break;
					case laser_Duty_Bit:
						f = laserPWM.frequency;
						f = 10000/f;
						d = f/100*laserPWM.duty;
						__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, d);  //���ñȽ�ֵ
						xEventGroupClearBits(laserGroupHandler,laser_Duty_Bit);
						break;
					default:
						break;
					
					
				}

			}
			  osDelay(1);

  	}
  }
  /* USER CODE END temCollectTask */


