#ifndef _CO2_H_
#define _CO2_H_
#include "main.h"

#define SlaveAddr		0x01			/* �����Ϊʱ���������ӻ� */


#define REG_HA01		0x5001
#define REG_HAXX		REG_A01

/* RTU Ӧ����� */
#define RSP_OK				0		/* �ɹ� */
#define RSP_ERR_CMD			0x01	/* ��֧�ֵĹ����� */
#define RSP_ERR_REG_ADDR	0x02	/* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE		0x03	/* ����ֵ����� */
#define RSP_ERR_WRITE		0x04	/* д��ʧ�� */

#define H_RX_BUF_SIZE		64
#define H_TX_BUF_SIZE      	128

extern float CO2_Concentration;



typedef struct
{
	uint8_t RxBuf[H_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[H_TX_BUF_SIZE];
	uint8_t TxCount;
	
	uint16_t Reg01H;		/* �����������͵ļĴ����׵�ַ */
	uint16_t Reg02H;
	uint16_t Reg03H;		
	uint16_t Reg04H;

	uint8_t RegNum;			/* �Ĵ������� */

	uint8_t fAck01H;		/* Ӧ�������־ 0 ��ʾִ��ʧ�� 1��ʾִ�гɹ� */
	uint8_t fAck02H;
	uint8_t fAck03H;
	uint8_t fAck04H;
	uint8_t fAck05H;		
	uint8_t fAck06H;		
	uint8_t fAck10H;
	
}MODH_T;

//typedef struct
//{
//	/* 03H 06H ��д���ּĴ��� */
//	uint16_t P01;
//	uint16_t P02;
//	
//	/* 02H ��д��ɢ����Ĵ��� */
//	uint16_t T01;
//	uint16_t T02;
//	uint16_t T03;
//	
//	/* 04H ��ȡģ�����Ĵ��� */
//	uint16_t A01;
//	
//	/* 01H 05H ��д����ǿ����Ȧ */
//	uint16_t D01;
//	uint16_t D02;
//	uint16_t D03;
//	uint16_t D04;
//	
//}VAR_T;

void MODH_Poll(void);

void MODH_ReciveNew(uint8_t _data);

uint8_t MODH_ReadParam_01H(uint16_t _reg, uint16_t _num);
uint8_t MODH_ReadParam_02H(uint16_t _reg, uint16_t _num);
uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num);
uint8_t MODH_ReadParam_04H(uint16_t _reg, uint16_t _num);
uint8_t MODH_WriteParam_05H(uint16_t _reg, uint16_t _value);
uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value);
uint8_t MODH_WriteParam_10H(uint16_t _reg, uint8_t _num, uint8_t *_buf);

extern MODH_T g_tModH;


void CO2UpdateTask(void const * argument);

#endif
