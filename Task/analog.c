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


SemaphoreHandle_t singleDateDealSemaphore;	//激光器PWM定时器溢出中断
analog_para per;

static TimerHandle_t	AutoReloadTimer_Handle;			//单次定时器句柄

static uint8_t ticks;  //定时器节拍数  1个节拍1ms
static uint8_t cnt_H = 0; //高电平计数
static uint8_t cnt_L = 0; //低电平计数
static float tem_value_H[120]; //高电平数据缓存
static float tem_value_L[120]; //低电平数据缓存

static void AutoReloadCallback(TimerHandle_t xTimer)  //软件定时器回调函数,读取PWM高低电平时的AD值
{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == 0)  // 高电平

		{
			tem_value_H[cnt_H++] = hal_ADC_M4_get_value(SINGLE_CH);
			if(cnt_H>120)   //防溢出，溢出后触发计算
			{
			xSemaphoreGive(singleDateDealSemaphore);	//释放二值信号量
			}

		}
		else     //低电平 
		{
			tem_value_L[cnt_L++] = hal_ADC_M4_get_value(SINGLE_CH);
			if(cnt_L>120)   //防溢出，溢出后触发计算
			{
			xSemaphoreGive(singleDateDealSemaphore);	//释放二值信号量
			}


		}
}



void analogCollect_Init()
{

	AutoReloadTimer_Handle=xTimerCreate((const char*			)"AutoReloadTimer", 
										 (TickType_t			)1,
										 (UBaseType_t			)pdTRUE,
										 (void*					)1,
										 (TimerCallbackFunction_t)AutoReloadCallback); //定时器,周期初始化1ms
	ASSERT(AutoReloadTimer_Handle);
										 
	singleDateDealSemaphore=xSemaphoreCreateBinary();	 //激光器PWM定时器溢出中断信号量
										 
	xTimerStart(AutoReloadTimer_Handle,0);  //开启软件定时器计数

}


// 优化,周期内取100点做平均,舍弃前后,如果采样周期小于1ms，即定为1ms 能取多少取多少。
void analogCollectTask(void const * argument)
{
  /* USER CODE BEGIN signalCollectTask */
  /* Infinite loop */

	uint8_t i;
  float value = 0;	 			//采集值加和
	float H_value_av = 0;  //高电平平均采集值
	float L_value_av = 0;	//低电平平均采集值

	BaseType_t err=pdFALSE;

  for(;;)
  {
		if(((1000/laserPWM.frequency)*(laserPWM.duty/100.0))>100)  //通过频率、占空比判断AD采样周期 高电平或低电平大于100ms,5ms读一个数，否则1ms
		{
			ticks = 5;
		}
		else 
			ticks = 1;

		if(singleDateDealSemaphore!=NULL)
		{
			
			err=xSemaphoreTake(singleDateDealSemaphore,portMAX_DELAY);	//获取信号量，PWM一个周期结束
			if(err==pdTRUE)	//获取信号量成功
			{
				HAL_TIM_Base_Stop_IT(&htim4);  //关闭定时器
				HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);//关闭PWM通道2
				xTimerStop(AutoReloadTimer_Handle,0); //关闭定时器
				xTimerChangePeriod(AutoReloadTimer_Handle,ticks,0); //改变一次采样周期，避免频率周期变化
				if((cnt_L>(delayMs/ticks)) && (cnt_H>(delayMs/ticks))) //高低电平计数大于要去除的数
				{
						for(i = delayMs/ticks; i <= (cnt_L-(delayMs/ticks)); i++) //去掉前后delayMs时间的数据
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
				else //高低电平计数小于要去除的数，直接做平均
				{
						for(i = 0; i <= cnt_L; i++) //去掉前后delayMs时间的数据
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
					
				}  //高电平大于低电平信号则做差

				cnt_H =0;
				cnt_L =0;
				
				HAL_TIM_Base_Start_IT(&htim4); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE
				HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//开启PWM通道2
				xTimerStart(AutoReloadTimer_Handle,0); //开启定时器
			}					
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

