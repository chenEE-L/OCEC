#include "analog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "debug_assert.h"
#include "laser.h"
#include "tim.h"

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


#define delayMs 10


SemaphoreHandle_t singleDateDealSemaphore;	//������PWM��ʱ������ж�
analog_para per;

static TimerHandle_t	AutoReloadTimer_Handle;			//���ζ�ʱ�����

static uint8_t ticks;  //��ʱ��������  1������1ms
static uint8_t cnt_H = 0; //�ߵ�ƽ����
static uint8_t cnt_L = 0; //�͵�ƽ����
static float tem_value_H[120]; //�ߵ�ƽ���ݻ���
static float tem_value_L[120]; //�͵�ƽ���ݻ���

static void AutoReloadCallback(TimerHandle_t xTimer)  //�����ʱ���ص�����,��ȡPWM�ߵ͵�ƽʱ��ADֵ
{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)  // �ߵ�ƽ

		{
			tem_value_H[cnt_H++] = hal_ADC_M4_get_value(SINGLE_CH);
			if(cnt_H>120)   //�����������󴥷�����
			{
			xSemaphoreGive(singleDateDealSemaphore);	//�ͷŶ�ֵ�ź���
			}

		}
		else     //�͵�ƽ 
		{
			tem_value_L[cnt_L++] = hal_ADC_M4_get_value(SINGLE_CH);
			if(cnt_L>120)   //�����������󴥷�����
			{
			xSemaphoreGive(singleDateDealSemaphore);	//�ͷŶ�ֵ�ź���
			}


		}
}



void analogCollect_Init()
{

	AutoReloadTimer_Handle=xTimerCreate((const char*			)"AutoReloadTimer", 
										 (TickType_t			)1,
										 (UBaseType_t			)pdTRUE,
										 (void*					)1,
										 (TimerCallbackFunction_t)AutoReloadCallback); //��ʱ��,���ڳ�ʼ��1ms
	ASSERT(AutoReloadTimer_Handle);
										 
	singleDateDealSemaphore=xSemaphoreCreateBinary();	 //������PWM��ʱ������ж��ź���
										 
	xTimerStart(AutoReloadTimer_Handle,0);  //���������ʱ������

}


// �Ż�,������ȡ100����ƽ��,����ǰ��,�����������С��1ms������Ϊ1ms ��ȡ����ȡ���١�
void analogCollectTask(void const * argument)
{
  /* USER CODE BEGIN signalCollectTask */
  /* Infinite loop */

	uint8_t i;
  float value = 0;	 			//�ɼ�ֵ�Ӻ�
	float H_value_av = 0;  //�ߵ�ƽƽ���ɼ�ֵ
	float L_value_av = 0;	//�͵�ƽƽ���ɼ�ֵ

	BaseType_t err=pdFALSE;

  for(;;)
  {
		if(((1000/laserPWM.frequency)*(laserPWM.duty/100.0))>100)  //ͨ��Ƶ�ʡ�ռ�ձ��ж�AD�������� �ߵ�ƽ��͵�ƽ����100ms,5ms��һ����������1ms
		{
			ticks = 5;
		}
		else 
			ticks = 1;

		if(singleDateDealSemaphore!=NULL)
		{
			
			err=xSemaphoreTake(singleDateDealSemaphore,portMAX_DELAY);	//��ȡ�ź�����PWMһ�����ڽ���
			if(err==pdTRUE)	//��ȡ�ź����ɹ�
			{
				HAL_TIM_Base_Stop_IT(&htim4);  //�رն�ʱ��
				HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);//�ر�PWMͨ��2
				xTimerStop(AutoReloadTimer_Handle,0); //�رն�ʱ��
				xTimerChangePeriod(AutoReloadTimer_Handle,ticks,0); //�ı�һ�β������ڣ�����Ƶ�����ڱ仯
				if((cnt_L>(delayMs/ticks)) && (cnt_H>(delayMs/ticks))) //�ߵ͵�ƽ��������Ҫȥ������
				{
						for(i = delayMs/ticks; i <= (cnt_L-(delayMs/ticks)); i++) //ȥ��ǰ��delayMsʱ�������
					{
						value = value+tem_value_L[i];
						
					}
					
					L_value_av = value/(cnt_L-2*(delayMs/ticks));
					value = 0;

						for(i = (delayMs/ticks); i <= (cnt_H-(delayMs/ticks)); i++)
					{
						value = value+tem_value_H[i];
						
					}
					H_value_av = value/(cnt_H-2*(delayMs/ticks));
					
					value = 0; //
				}
				else //�ߵ͵�ƽ����С��Ҫȥ��������ֱ����ƽ��
				{
						for(i = 0; i <= cnt_L; i++) //ȥ��ǰ��delayMsʱ�������
					{
						value = value+tem_value_L[i];
						
					}
					
					L_value_av = value/cnt_L;
					value = 0;

						for(i = 0; i <= cnt_H; i++)
					{
						value = value+tem_value_H[i];
						
					}
					H_value_av = value/cnt_H;
					
					value = 0; //
				}
					
				if(H_value_av > L_value_av){
				per.signal = (H_value_av - L_value_av)*10000; //100uV
					
				}  //�ߵ�ƽ���ڵ͵�ƽ�ź�������

				cnt_H =0;
				cnt_L =0;
				
				HAL_TIM_Base_Start_IT(&htim4); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE
				HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//����PWMͨ��2
				xTimerStart(AutoReloadTimer_Handle,0); //������ʱ��
			}					
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

