#ifndef _LASER_H_
#define _LASER_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define laser_Frequency_Bit	(1<<0)				//事件位
#define laser_Duty_Bit	    (1<<1)				//事件位

#define EVENTBIT_ALL_LASER	(laser_Frequency_Bit|laser_Duty_Bit)

extern EventGroupHandle_t laserGroupHandler;	//事件标志组句柄

typedef struct
{
	//读参数
	uint16_t frequency;
	uint16_t duty ;
	
}laser_para;

extern laser_para laserPWM;

void laser_init(void);

void laserControlTask(void const * argument);



#endif
