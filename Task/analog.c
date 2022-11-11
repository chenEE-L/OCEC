#include "analog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"

/* USER CODE BEGIN Header_signalCollectTask */
/**
* @brief Function implementing the signalCollect thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_signalCollectTask */

#define Rg        (4300.0f)
#define v_ref     (3.0f)

#define v_refpt   (3.0f/(1000.0f+1000.0f)*1000.0f) 

#define amp       (49.4f/Rg+1.0f)



analog_para per;


void analogCollectTask(void const * argument)
{
  /* USER CODE BEGIN signalCollectTask */
  /* Infinite loop */
//	float temp;
	float tem_value = 0;
	static char state = 0;
	static uint16_t cnt = 0;
	static float H_value = 0;
	static float L_value = 0;
	static float H_value_av = 0;
	static float L_value_av = 0;

  for(;;)
  {

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
		{
			if(state == 1)
			{
				L_value_av = L_value/cnt;
				
				if((H_value_av - L_value_av)>0){
				per.signal = (H_value_av - L_value_av)*1000;
				}
				
				H_value = 0;
				L_value = 0;
				H_value_av = 0;
				L_value_av = 0;
				
				cnt = 0;
				tem_value = 0;
				state = 0;
			}
			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
			H_value = H_value+tem_value;
			cnt++;
		}
		else
		{
			if(state == 0)
			{
				H_value_av = H_value/cnt;
				cnt = 0;
				tem_value = 0;
				state = 1;
			}
			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
			L_value = L_value+tem_value;
			cnt++;
		}
		
//		per.signal = hal_ADC_M4_get_value(SINGLE_CH);
		//计算PT阻值
//		temp = hal_ADC_M4_get_value(PT1000_CH);
//		temp = temp/amp+v_refpt;
//		temp = temp/((v_ref-temp)/1000.0f);
		// PT阻值温度转换
//		per.tem = hal_ADC_M4_get_value(PT1000_CH);
		
		/*********************************/
		
		osDelay(1);
		
		}
    
 }
  /* USER CODE END signalCollectTask */

