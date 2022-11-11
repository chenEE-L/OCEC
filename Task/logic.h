#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

//#define logic_sample_Bit			(1<<0)				//事件位
//#define logic_purge_Bit				(1<<1)				//事件位
//#define logic_OC_Bit					(1<<2)				//事件位
//#define logic_EC_Bit					(1<<3)				//事件位
//#define logic_ration_Bit			(1<<4)				//事件位
//#define logic_calibration_Bit	(1<<5)				//事件位

#define logic_sample_start			(1<<0)				//事件位
#define logic_sample_end  			(1<<1)				//事件位
#define logic_purge_start				(1<<2)				//事件位
#define logic_purge_end     		(1<<3)				//事件位
#define logic_OC_start					(1<<4)				//事件位
#define logic_OC_end  					(1<<5)				//事件位
#define logic_EC_start					(1<<6)				//事件位
#define logic_EC_end				    (1<<7)				//事件位
#define logic_ration_start			(1<<8)				//事件位
#define logic_ration_end			  (1<<9)				//事件位
#define logic_calibration_start	(1<<10)				//事件位
#define logic_calibration_end 	(1<<11)				//事件位

#define EVENTBIT_ALL_LOGIC	(logic_sample_start|logic_sample_end|logic_purge_start|logic_purge_end|logic_OC_start|logic_OC_end|\
														 logic_EC_start|logic_EC_end|logic_ration_start|logic_ration_end|logic_calibration_start|logic_calibration_end)

extern EventGroupHandle_t logicGroupHandler;	//事件标志组句柄
void logic_init(void);
#endif

