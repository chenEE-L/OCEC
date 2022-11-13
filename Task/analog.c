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



//冒泡排序
//void BubbleSort(int *arry,int len)
//{
//	int i;
//	int j;
//	int temp;
//	for(i=0;i<len-1;i++)//比较次数
//	{
//					for(j=0;j<len-1-i;j++)//比较过程
//					{
//									if(arry[j]>arry[j+1]) //比较大小
//									{
//													temp=arry[j];
//													arry[j]=arry[j+1];
//													arry[j+1]=temp;

//									}
//					}

//	}

//}

// 优化,周期内取100点做平均,舍弃前后,如果采样周期小于1ms，即定为1ms 能取多少取多少。
void analogCollectTask(void const * argument)
{
  /* USER CODE BEGIN signalCollectTask */
  /* Infinite loop */
//	float temp;
	float tem_value = 0;		 			//存放单次采集值
	static char state = 0;   			//翻转首次标志位
	static uint16_t cnt = 0; 			//采集次数
	static float value = 0;	 			//采集值加和
	static float H_value_av = 0;  //高电平平均采集值
	static float L_value_av = 0;	//低电平平均采集值

  for(;;)
  {

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
		{
			if(state == 1)
			{
				L_value_av = value/cnt;
				
				if((H_value_av - L_value_av)>0){
				per.signal = (H_value_av - L_value_av)*1000;
				}  //高电平大于低电平信号则做差
				
				value = 0;
				H_value_av = 0;
				L_value_av = 0;
				cnt = 0;
				tem_value = 0;
				state = 0;     //置零相关参数，开始下一次计算
			}
			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
			value = value+tem_value;
			cnt++;  
		}
		else
		{
			if(state == 0)
			{
				H_value_av = value/cnt;
				value = 0;
				cnt = 0;
				tem_value = 0;
				state = 1;
			}
			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
			value = value+tem_value;
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

//void analogCollectTask(void const * argument)
//{
//  /* USER CODE BEGIN signalCollectTask */
//  /* Infinite loop */
////	float temp;
//	float tem_value = 0;		 			//存放单次采集值
//	static char state = 0;   			//翻转首次标志位
//	static uint16_t cnt = 0; 			//采集次数
//	static float value = 0;	 			//采集值加和
//	static float H_value_av = 0;  //高电平平均采集值
//	static float L_value_av = 0;	//低电平平均采集值

//  for(;;)
//  {

//		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
//		{
//			if(state == 1)
//			{
//				L_value_av = value/cnt;
//				
//				if((H_value_av - L_value_av)>0){
//				per.signal = (H_value_av - L_value_av)*1000;
//				}  //高电平大于低电平信号则做差
//				
//				value = 0;
//				H_value_av = 0;
//				L_value_av = 0;
//				cnt = 0;
//				tem_value = 0;
//				state = 0;     //置零相关参数，开始下一次计算
//			}
//			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
//			value = value+tem_value;
//			cnt++;  
//		}
//		else
//		{
//			if(state == 0)
//			{
//				H_value_av = value/cnt;
//				value = 0;
//				cnt = 0;
//				tem_value = 0;
//				state = 1;
//			}
//			tem_value = hal_ADC_M4_get_value(SINGLE_CH);
//			value = value+tem_value;
//			cnt++;
//		}
//		
////		per.signal = hal_ADC_M4_get_value(SINGLE_CH);
//		//计算PT阻值
////		temp = hal_ADC_M4_get_value(PT1000_CH);
////		temp = temp/amp+v_refpt;
////		temp = temp/((v_ref-temp)/1000.0f);
//		// PT阻值温度转换
////		per.tem = hal_ADC_M4_get_value(PT1000_CH);
//		
//		/*********************************/
//		
//		osDelay(1);
//		
//		}
//    
// }
  /* USER CODE END signalCollectTask */

