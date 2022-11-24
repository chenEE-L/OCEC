#ifndef _MODS_H_
#define _MODS_H_

#include "main.h"

#define SADDR	1

/* 01H ��ǿ�Ƶ���Ȧ */
/* 05H дǿ�Ƶ���Ȧ */
#define REG_D01		0x0101
#define REG_D02		0x0102
#define REG_D03		0x0103
#define REG_D04		0x0104
#define REG_D05		0x0105
#define REG_D06		0x0106
#define REG_D07		0x0107
#define REG_D08		0x0108
#define REG_D09		0x0109
#define REG_D010	0x010A

// �߼����
#define REG_D11		0x0110
#define REG_D12		0x0111
#define REG_D13		0x0112
#define REG_D14	  0x0113
#define REG_D15   0x0114
#define REG_D16   0x0115
#define REG_DXX 	REG_D16

/* 02H ��ȡ����״̬ */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H �����ּĴ��� */
/* 06H д���ּĴ��� */
/* 10H д�������Ĵ��� */
#define SLAVE_REG_P01		0x0301
#define SLAVE_REG_P02		0x0302

#define SLAVE_REG_P03		0x0303
#define SLAVE_REG_P04		0x0304

#define SLAVE_REG_P05		0x0305
#define SLAVE_REG_P06		0x0306

#define SLAVE_REG_P07		0x0307
#define SLAVE_REG_P08		0x0308

#define SLAVE_REG_P09		0x0309
#define SLAVE_REG_P10	  0x030A

#define SLAVE_REG_P11	  0x030B

#define SLAVE_REG_P12   0x030C

/* 04H ��ȡ����Ĵ���(ģ���ź�) */
#define REG_A01		0x0401  // �ź�ֵ
//PT1000 
#define REG_A02   0x0402 
#define REG_A03   0x0403 

#define REG_A04   0x0404 
#define REG_A05   0x0405 

#define REG_AXX		REG_A05



/* RTU Ӧ����� */
#define RSP_OK				0		/* �ɹ� */
#define RSP_ERR_CMD			0x01	/* ��֧�ֵĹ����� */
#define RSP_ERR_REG_ADDR	0x02	/* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE		0x03	/* ����ֵ����� */
#define RSP_ERR_WRITE		0x04	/* д��ʧ�� */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128




typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H ��д���ּĴ��� */
	uint16_t flowA;
	uint16_t flowB;
	uint16_t flowC;
	uint16_t flowD;
	uint16_t flowE;
	


	uint16_t frequency;
	uint16_t duty;
	

	/* 04H ��ȡģ�����Ĵ��� */
	uint16_t signal;
	uint16_t pt1000;
	/* 01H 05H ��д����ǿ����Ȧ */
	uint8_t D01;
	uint8_t D02;
	uint8_t D03;
	uint8_t D04;
	uint8_t D05;
	uint8_t D06;
	uint8_t D07;
	uint8_t D08;
	uint8_t D09;
	uint8_t D010;
	
}VAR_T;

extern MODS_T g_tModS;
extern VAR_T g_tVar;


void MODS_Poll(void);
void MODS_ReciveNew(uint8_t _byte);
void modsUpdateTask(void const * argument);

#endif
