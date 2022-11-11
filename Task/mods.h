#ifndef _MODS_H_
#define _MODS_H_

#include "modbus_slave.h"
#include "flow.h"
#include "analog.h"
#include "laser.h"
extern TimerHandle_t	OneShotTimer_Handle;			//单次定时器句柄



void MODSInit(void);

void modsUpdateTask(void const * argument);

#endif
