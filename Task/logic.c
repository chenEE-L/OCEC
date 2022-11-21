#include "logic.h"
#include "cmsis_os.h"
#include "debug_assert.h"
#include "flow.h"
#include "gpio.h"
EventGroupHandle_t logicGroupHandler;	//�¼���־����


void logic_init()
{
	
	logicGroupHandler=xEventGroupCreate();	
	ASSERT(logicGroupHandler);

}

void logicControTask(void const * argument)
{
	EventBits_t EventValue;
	closeAllFlow(); // ����״̬����ȫ��
  for(;;)
  {
		if(logicGroupHandler!=NULL)
		{
			EventValue=xEventGroupGetBits(logicGroupHandler);	//��ȡ�¼���
			switch((uint16_t)EventValue)
			{
				case logic_sample_start:
					//��6����
					setFlow(&DeviceE,8);
				xEventGroupClearBits(logicGroupHandler,logic_sample_start);
					break;
				case logic_sample_end:
					//��6�ر�
					setFlow(&DeviceE,0);
				xEventGroupClearBits(logicGroupHandler,logic_sample_end);
					break;
				case logic_purge_start:
					setFlow(&DeviceD,0.25);
					setFlow(&DeviceC,0.7);
				xEventGroupClearBits(logicGroupHandler,logic_purge_start);
					//��5��
					break;
				case logic_purge_end:
					setFlow(&DeviceD,0);
					setFlow(&DeviceC,0);
				xEventGroupClearBits(logicGroupHandler,logic_purge_end);
					//��5�ر�
					break;
				case logic_OC_start:
					setFlow(&DeviceD,0.25);
					setFlow(&DeviceC,0.7);
				xEventGroupClearBits(logicGroupHandler,logic_OC_start);
					//��5��
					break;
				case logic_OC_end:
					setFlow(&DeviceD,0);
					setFlow(&DeviceC,0);
				xEventGroupClearBits(logicGroupHandler,logic_OC_end);
					//��5�ر�
					break;
				case logic_EC_start:
					setFlow(&DeviceD,0.25);
					setFlow(&DeviceC,0.40);
					setFlow(&DeviceB,0.30);
				  bsp_DoOn(2);
				xEventGroupClearBits(logicGroupHandler,logic_EC_start);
					//��2����
					break;
				case logic_EC_end:
					setFlow(&DeviceD,0);
					setFlow(&DeviceC,0);
					setFlow(&DeviceB,0);
					bsp_DoOff(2);
				xEventGroupClearBits(logicGroupHandler,logic_EC_end);
					//��2�ر�
					break;
				case logic_ration_start:
					setFlow(&DeviceD,0.25);
					setFlow(&DeviceC,0.40);
					setFlow(&DeviceB,0.30);
					setFlow(&DeviceA,0.15);
				xEventGroupClearBits(logicGroupHandler,logic_ration_start);
					//��2����
					break;
				case logic_ration_end:
					setFlow(&DeviceD,0);
					setFlow(&DeviceC,0);
					setFlow(&DeviceB,0);
					setFlow(&DeviceA,0);
				xEventGroupClearBits(logicGroupHandler,logic_ration_end);
					//��2�ر�
					break;
				case logic_calibration_start:
					setFlow(&DeviceD,0.25);
					setFlow(&DeviceC,0.40);
					setFlow(&DeviceB,0.30);
					setFlow(&DeviceA,0);
				xEventGroupClearBits(logicGroupHandler,logic_calibration_start);
					//��1��3��4����
					break;
				case logic_calibration_end:
					setFlow(&DeviceD,0);
					setFlow(&DeviceC,0);
					setFlow(&DeviceB,0);
					setFlow(&DeviceA,0);
				xEventGroupClearBits(logicGroupHandler,logic_calibration_end);
					//��1��3��4�ر�
					break;
				default:
					break;
				
				
			}
//			xEventGroupClearBits(logicGroupHandler,EVENTBIT_ALL_LOGIC);

		}

	  osDelay(1);
  }

}
