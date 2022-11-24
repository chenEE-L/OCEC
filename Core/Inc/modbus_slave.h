/*
*********************************************************************************************************
*
*	模块名称 : MODEBUS 通信模块 (从站）
*	文件名称 : modbus_slave.h
*	说    明 : 头文件
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

