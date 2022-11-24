/*
*********************************************************************************************************
*
*	ģ������ : MODEBUS ͨ��ģ�� (��վ��
*	�ļ����� : modbus_slave.h
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/


#ifndef __MODBUY_SLAVE_H
#define __MODBUY_SLAVE_H

#include "main.h"


extern float write_hex_to_float(uint32_t number);
extern uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
extern uint16_t BEBufToUint16(uint8_t *_pBuf);

#endif

