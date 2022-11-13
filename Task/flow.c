#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "flow.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "debug_assert.h"

/* USER CODE BEGIN Header_flowCollectTask */
/**
* @brief Function implementing the flowCollect thread.
* @param argument: Not used
* @retval None
*/



#define Device_NUM 5 // �����Ƹ���
#define CMD_bNUM  5  //��ȡ������ֽ���


EventGroupHandle_t flowGroupHandler;	//�¼���־����

flow_para DeviceA = {
	.addr = 'A',
	.span = 1
}; 
flow_para DeviceB = {
	.addr = 'B',
	.span = 1
}; 
flow_para DeviceC = {
	.addr = 'C',
	.span = 1
}; 
flow_para DeviceD = {
	.addr = 'D',
	.span = 1
}; 
flow_para DeviceE = {
	.addr = 'E',
	.span = 20
};  //��ʼ���豸const����

static void atkpAnalyze(uint8_t *p);
static int gps_extract_field(char *buff,char *field[],char delimiter,short field_no_max);

static enum
{
	waitForID,
	waitForData,
}rxState;  //�����ƽ���switch���


static uint8_t readCmd[Device_NUM][CMD_bNUM] =  {{'A',0x0d,0x0a,0x0d,'\0'},
																								 {'B',0x0d,0x0a,0x0d,'\0'},
																								 {'C',0x0d,0x0a,0x0d,'\0'},
																								 {'D',0x0d,0x0a,0x0d,'\0'},
																								 {'E',0x0d,0x0a,0x0d,'\0'}};// �����ƶ�ȡ����


void flow_init()
{
	DeviceA.rx_para.pressure = 0;
	DeviceA.rx_para.set_value_r = 0;
	DeviceA.set_value_w = 0;
	
	DeviceB.rx_para.pressure = 0;
	DeviceB.rx_para.set_value_r = 0;
	DeviceB.set_value_w = 0;
	
	DeviceC.rx_para.pressure = 0;
	DeviceC.rx_para.set_value_r = 0;
	DeviceC.set_value_w = 0;
	
	DeviceD.rx_para.pressure = 0;
	DeviceD.rx_para.set_value_r = 0;
	DeviceD.set_value_w = 0;
	
	DeviceE.rx_para.pressure = 0;
	DeviceE.rx_para.set_value_r = 0;
	DeviceE.set_value_w = 0;     //  �����Ʋ�����ʼ�� 
	
	
	flowGroupHandler=xEventGroupCreate();	// ���������¼���
	ASSERT(flowGroupHandler);
	
}





/* USER CODE END Header_flowCollectTask */
void flowCollectTask(void const * argument)  //������������ȡ����,������Ҫȷ���ɼ����ڣ�
{
  /* USER CODE BEGIN flowCollectTask */
  /* Infinite loop */	
	uint8_t tx_flag = 1; //��ȡ��������ָ��ͱ�־λ,��1Ϊ������������ݣ��ɻ�ȡ��һ�豸״̬
	uint8_t c;
	uint8_t date[100];
	uint8_t i = 0;
	uint8_t errcnt = 0;
  uint32_t command_data_length=0;
	rxState = waitForID;
	
  for(;;)
  {
		
		if(tx_flag)
		{
			tx_flag = 0;
			if(i>=Device_NUM) i = 0; //�豸��ȡ��ϣ����¶�ȡ�׸��豸����
			HAL_UART_Transmit(&huart4,readCmd[i],CMD_bNUM-1,1000);	//����ID��ȡ����������
   		while(__HAL_UART_GET_FLAG(&huart4,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
		}
		
		if (COMGetDataWithTimout(COM4DataDelivery,&c))  // �������ݽ���
		{
			switch(rxState)
			{
				case waitForID:
					if(c == readCmd[i][0])
					{
						rxState = waitForData;
						date[command_data_length++] = c;
					}
					else
						rxState = waitForID;
					break;
				case waitForData:
					date[command_data_length++] = c;
					if((c ==0x0a)||(c ==0x0d)) //�����������
					{
						atkpAnalyze(date);  //��������
						rxState = waitForID;
						command_data_length = 0;
						tx_flag = 1;
						i++;
						osDelay(50); // ��ʱ�Ƿ��Ҫ����ȥ����������
					}
					break;
				default:
					break;
			}
		}
		else
		{
			errcnt ++;
			tx_flag = 1;  //δ��ȡ�������¶�ȡ
			if(errcnt >3) //3��δ��ȡ��,��Ϊ���豸δ�ӻ����𻵣���ȡ��һ�豸
			{
				i++;
				errcnt =0;
			}
		}
			osDelay(1); 
	}
}

static void atkpAnalyze(uint8_t *p)
{
	struct para flow_date;
	int command_num;
	char *command_hold1[SERIAL_COMMAND_HOLD_LENGTH];
	char nowaddr; //��ʱ��Ŷ�ȡ��ַ����������ж�
	command_num = gps_extract_field((char*)p,command_hold1,' ',SERIAL_COMMAND_HOLD_LENGTH);  //�ַ����ָ�
	
	nowaddr = command_hold1[0][0];
	flow_date.pressure = atof(&command_hold1[1][1]);//�ַ���ת������
	flow_date.temp = atof(&command_hold1[2][1]);
	flow_date.cubage = atof(&command_hold1[3][1]);
//			flow_date.quality = even_filter_float(&Alicat_filter1,atof(&command_hold1[4][1]));//�˲�
	flow_date.quality = atof(&command_hold1[4][1]);
	flow_date.set_value_r = atof(&command_hold1[5][1]);
	strcpy(flow_date.gas_name,command_hold1[command_num-1]);
	switch(nowaddr)
	{
		case 'A':
			DeviceA.rx_para= flow_date;
			break;
		case'B':
			DeviceB.rx_para = flow_date;
			break;
		case'C':
			DeviceC.rx_para = flow_date;
			break;
		case'D':
			DeviceD.rx_para = flow_date;
			break;
		case'E':
			DeviceE.rx_para = flow_date;
			break;
		default:
			break;
	}
	
}




static int gps_extract_field(char *buff,char *field[],char delimiter,short field_no_max)
{
   int i,field_no,current_field_pos,len;
   
   len = strlen(buff);
   if(len <= 0) return(0);             // no chars in string
   field_no=0;                                // field index 0..n-1
   current_field_pos=0;                                // char index     
   
   for(i=0; i<len; i++)
   {   
   
      if(current_field_pos == 0)
      {
         field[field_no] = buff+i;           // point to start of string    
      }      
     
      if((i==len-1) || (buff[i]=='*'))
      {        
          // last field found 
          buff[i] = '\0';               // terminate field with null 
          return(field_no+1);
      }
      else if(buff[i]=='\0')
      {        
          // last field found 
          return(field_no);
      }
      else if(buff[i]== delimiter)
      { 
         // end of current field found  
         buff[i] = '\0';               // terminate field with null
         current_field_pos = 0;                     // first char
         field_no++;                       // of next field        
      }
      else
      {
        current_field_pos++;                        // move to next character  
      } 
      
      if(field_no == field_no_max)
      {
         return(field_no);            // out of pointers
      }
   }
   return(field_no);
}    


void flowSetTask(void const * argument)  //���ݷ������¼����־������Ӧ�豸������
{
		EventBits_t EventValue;

	  for(;;)
  {
		if(flowGroupHandler!=NULL)
		{
			EventValue=xEventGroupGetBits(flowGroupHandler);	//��ȡ�¼����
			switch((uint8_t)EventValue)
			{
				case flow1_Device_Bit:
					setFlow(DeviceA,DeviceA.set_value_w);
				xEventGroupClearBits(flowGroupHandler,flow1_Device_Bit);
					break;
				case flow2_Device_Bit:
					setFlow(DeviceB,DeviceB.set_value_w);
				xEventGroupClearBits(flowGroupHandler,flow2_Device_Bit);
					break;
				case flow3_Device_Bit:
					setFlow(DeviceC,DeviceC.set_value_w);
				xEventGroupClearBits(flowGroupHandler,flow3_Device_Bit);
					break;
				case flow4_Device_Bit:
					setFlow(DeviceD,DeviceD.set_value_w);
				xEventGroupClearBits(flowGroupHandler,flow4_Device_Bit);
					break;
				case flow5_Device_Bit:
					setFlow(DeviceE,DeviceE.set_value_w);
				xEventGroupClearBits(flowGroupHandler,flow5_Device_Bit);
					break;
				default:
					break;
			}
//			xEventGroupClearBits(flowGroupHandler,EVENTBIT_ALL_FLOW);

		}

	  osDelay(10);

	}
			

}

static uint8_t alicat_init[10]={'A',0x36,0x34,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};



void setFlow(flow_para p ,float flow_rate)
{
	uint16_t flow;
	
	flow = (uint16_t)((flow_rate/p.span)*64000);
	if(flow>64000)
	{
			flow = 64000;
	}
	alicat_init[0] = p.addr;
	alicat_init[1] = flow/10000+0x30;
	alicat_init[2] = flow/1000%10+0x30;
	alicat_init[3] = flow/100%10+0x30;
	alicat_init[4] = flow/10%10+0x30;
	alicat_init[5] = flow%10+0x30;
	HAL_UART_Transmit(&huart4,alicat_init,10,1000);	//��������
}

void closeAllFlow()
{
	char i;
	
	for(i = 'A';i <= 'E';i++)
	{
	alicat_init[0] = i;
	alicat_init[1] = 0;
	alicat_init[2] = 0;
	alicat_init[3] = 0;
	alicat_init[4] = 0;
	alicat_init[5] = 0;
	HAL_UART_Transmit(&huart4,alicat_init,10,1000);
  osDelay(10);
	}
	
}



