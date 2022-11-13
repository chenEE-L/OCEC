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



//ð������
//void BubbleSort(int *arry,int len)
//{
//	int i;
//	int j;
//	int temp;
//	for(i=0;i<len-1;i++)//�Ƚϴ���
//	{
//					for(j=0;j<len-1-i;j++)//�ȽϹ���
//					{
//									if(arry[j]>arry[j+1]) //�Ƚϴ�С
//									{
//													temp=arry[j];
//													arry[j]=arry[j+1];
//													arry[j+1]=temp;

//									}
//					}

//	}

//}

// �Ż�,������ȡ100����ƽ��,����ǰ��,�����������С��1ms������Ϊ1ms ��ȡ����ȡ���١�
void analogCollectTask(void const * argument)
{
  /* USER CODE BEGIN signalCollectTask */
  /* Infinite loop */
//	float temp;
	float tem_value = 0;		 			//��ŵ��βɼ�ֵ
	static char state = 0;   			//��ת�״α�־λ
	static uint16_t cnt = 0; 			//�ɼ�����
	static float value = 0;	 			//�ɼ�ֵ�Ӻ�
	static float H_value_av = 0;  //�ߵ�ƽƽ���ɼ�ֵ
	static float L_value_av = 0;	//�͵�ƽƽ���ɼ�ֵ

  for(;;)
  {

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)
		{
			if(state == 1)
			{
				L_value_av = value/cnt;
				
				if((H_value_av - L_value_av)>0){
				per.signal = (H_value_av - L_value_av)*1000;
				}  //�ߵ�ƽ���ڵ͵�ƽ�ź�������
				
				value = 0;
				H_value_av = 0;
				L_value_av = 0;
				cnt = 0;
				tem_value = 0;
				state = 0;     //������ز�������ʼ��һ�μ���
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
		//����PT��ֵ
//		temp = hal_ADC_M4_get_value(PT1000_CH);
//		temp = temp/amp+v_refpt;
//		temp = temp/((v_ref-temp)/1000.0f);
		// PT��ֵ�¶�ת��
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
//	float tem_value = 0;		 			//��ŵ��βɼ�ֵ
//	static char state = 0;   			//��ת�״α�־λ
//	static uint16_t cnt = 0; 			//�ɼ�����
//	static float value = 0;	 			//�ɼ�ֵ�Ӻ�
//	static float H_value_av = 0;  //�ߵ�ƽƽ���ɼ�ֵ
//	static float L_value_av = 0;	//�͵�ƽƽ���ɼ�ֵ

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
//				}  //�ߵ�ƽ���ڵ͵�ƽ�ź�������
//				
//				value = 0;
//				H_value_av = 0;
//				L_value_av = 0;
//				cnt = 0;
//				tem_value = 0;
//				state = 0;     //������ز�������ʼ��һ�μ���
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
//		//����PT��ֵ
////		temp = hal_ADC_M4_get_value(PT1000_CH);
////		temp = temp/amp+v_refpt;
////		temp = temp/((v_ref-temp)/1000.0f);
//		// PT��ֵ�¶�ת��
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

