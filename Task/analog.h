#ifndef _ANALOG_H_
#define _ANALOG_H_

#define SINGLE_CH    1
#define PT1000_CH    0



typedef struct
{
	float signal;
	float tem;
	
}analog_para;

extern analog_para per;

void analogCollectTask(void const * argument);


#endif
