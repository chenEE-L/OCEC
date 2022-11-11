#ifndef _FLOW_H_
#define _FLOW_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define SERIAL_COMMAND_HOLD_LENGTH  20
#define ATKP_MAX_DATA_SIZE  100

#define flow1_Device_Bit	(1<<0)				//�¼�λ
#define flow2_Device_Bit	(1<<1)				//�¼�λ
#define flow3_Device_Bit	(1<<2)				//�¼�λ
#define flow4_Device_Bit	(1<<3)				//�¼�λ
#define flow5_Device_Bit	(1<<4)				//�¼�λ
#define EVENTBIT_ALL_FLOW	(flow1_Device_Bit|flow2_Device_Bit|flow3_Device_Bit|flow4_Device_Bit|flow5_Device_Bit)

extern EventGroupHandle_t flowGroupHandler;	//�¼���־����



typedef struct
{
	//������
	char addr;
//	const char span;
	float pressure;
	float temp;
	float cubage;
	float quality;
	float set_value_r;
	char gas_name[8];
	//д����
	float set_value_w;//�����������Ŵ�100��
}flow_para;

extern flow_para DeviceA; 
extern flow_para DeviceB;
extern flow_para DeviceC;
extern flow_para DeviceD;
extern flow_para DeviceE;

void flow_init(void);
void setFlow(flow_para p ,float flow_rate);
void flowCollectTask(void const * argument);
void flowSetTask(void const * argument);
void closeAllFlow(void);

#endif
