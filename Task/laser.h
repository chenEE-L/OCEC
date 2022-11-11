#ifndef _LASER_H_
#define _LASER_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define laser_Frequency_Bit	(1<<0)				//�¼�λ
#define laser_Duty_Bit	    (1<<1)				//�¼�λ

#define EVENTBIT_ALL_LASER	(laser_Frequency_Bit|laser_Duty_Bit)

extern EventGroupHandle_t laserGroupHandler;	//�¼���־����

typedef struct
{
	//������
	uint16_t frequency;
	uint16_t duty ;
	
}laser_para;

extern laser_para laserPWM;

void laser_init(void);

void laserControlTask(void const * argument);



#endif
