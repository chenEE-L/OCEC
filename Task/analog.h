#ifndef _ANALOG_H_
#define _ANALOG_H_
#include "FreeRTOS.h"
#include "semphr.h"

#define SINGLE_CH    1
#define PT1000_CH    0



typedef struct
{
	float signal;
	float tem;
	
}analog_para;

extern analog_para per;
extern SemaphoreHandle_t singleDateDealSemaphore;	//二值信号量句柄

void analogCollectTask(void const * argument);
void analogCollect_Init(void);


#endif
