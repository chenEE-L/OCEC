/*
*********************************************************************************************************
*
*	模块名称 : MODS通信模块. 从站模式
*	文件名称 : modbus_slave.c
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/
#include "modbus_slave.h"
#include <string.h>
#include "gpio.h"
#include "flow.h"
#include "mods.h"
#include "logic.h"
#include "tim.h"


const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC 低位字节值表
const uint8_t s_CRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};


static uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
static uint16_t BEBufToUint16(uint8_t *_pBuf);

static void MODS_RxTimeOut(void);


static void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen);
static void MODS_SendAckOk(void);
static void MODS_SendAckErr(uint8_t _ucErrCode);

static void MODS_AnalyzeApp(void);

//static void MODS_RxTimeOut(void);

static void MODS_01H(void);
//static void MODS_02H(void);
static void MODS_03H(void);
static void MODS_04H(void);
static void MODS_05H(void);
static void MODS_06H(void);
static void MODS_10H(void);

static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value);
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value);

static uint8_t g_mods_timeout = 0;
void MODS_ReciveNew(uint8_t _byte);

MODS_T g_tModS;
VAR_T g_tVar;
/*
*********************************************************************************************************
*	函 数 名: MODS_Poll
*	功能说明: 解析数据包. 在主程序中轮流调用。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/


 static union 
	{
	 float flo;
	 char buf[4];
	} HexFloat;

static float write_hex_to_float(uint32_t number)
	{
		uint16_t dat0 = number & 0x0000FFFF;
		uint16_t dat1 = (number >> 16) & 0x0000FFFF;
		HexFloat.buf[0] = dat0 & 0xFF;
		HexFloat.buf[1] = (dat0 >> 8) & 0xFF;
		HexFloat.buf[2] = dat1 & 0xFF;
		HexFloat.buf[3] = (dat1 >> 8) & 0xFF;
		float fdata = HexFloat.flo;

		return fdata;
	}
void MODS_Poll(void)
{
	uint16_t addr;
	uint16_t crc1;
	/* 超过3.5个字符时间后执行MODH_RxTimeOut()函数。全局变量 g_rtu_timeout = 1; 通知主程序开始解码 */
	if (g_mods_timeout == 0)	
	{
		return;								/* 没有超时，继续接收。不要清零 g_tModS.RxCount */
	}
	
	g_mods_timeout = 0;	 					/* 清标志 */

	if (g_tModS.RxCount < 4)				/* 接收到的数据小于4个字节就认为错误 */
	{
		goto err_ret;
	}

	/* 计算CRC校验和 */
	crc1 = CRC16_Modbus(g_tModS.RxBuf, g_tModS.RxCount);
	if (crc1 != 0)
	{
		goto err_ret;
	}

	/* 站地址 (1字节） */
	addr = g_tModS.RxBuf[0];				/* 第1字节 站号 */
	if (addr != SADDR)		 			/* 判断主机发送的命令地址是否符合 */
	{
		goto err_ret;
	}

	/* 分析应用层协议 */
	MODS_AnalyzeApp();						
	
err_ret:
#if 0										/* 此部分为了串口打印结果,实际运用中可不要 */
	g_tPrint.Rxlen = g_tModS.RxCount;
	memcpy(g_tPrint.RxBuf, g_tModS.RxBuf, g_tModS.RxCount);
#endif
	
	g_tModS.RxCount = 0;					/* 必须清零计数器，方便下次帧同步 */
}

/*
*********************************************************************************************************
*	函 数 名: MODS_ReciveNew
*	功能说明: 串口接收中断服务程序会调用本函数。当收到一个字节时，执行一次本函数。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MODS_ReciveNew(uint8_t _byte)
{
	uint32_t timeout;

//	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	g_mods_timeout = 0;
	timeout = 35000000 / 115200;			/* 计算超时时间，单位us 35000000*/

	/* 软件定时中断 */
//	xTimerStartFromISR(OneShotTimer_Handle ,&xHigherPriorityTaskWoken) ;
	bsp_StartHardTimer(timeout, (void *)MODS_RxTimeOut);

	if (g_tModS.RxCount < S_RX_BUF_SIZE)
	{
		g_tModS.RxBuf[g_tModS.RxCount++] = _byte;
	}
}

///*
//*********************************************************************************************************
//*	函 数 名: MODS_RxTimeOut
//*	功能说明: 超过3.5个字符时间后执行本函数。 设置全局变量 g_mods_timeout = 1; 通知主程序开始解码。
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
static void MODS_RxTimeOut(void)
{
	g_mods_timeout = 1;
}

//void OneShotCallback(TimerHandle_t xTimer)
//{
//	g_mods_timeout = 1;

//}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendWithCRC
*	功能说明: 发送一串数据, 自动追加2字节CRC
*	形    参: _pBuf 数据；
*			  _ucLen 数据长度（不带CRC）
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen)
{
	uint16_t crc;
	uint8_t buf[S_TX_BUF_SIZE];

	memcpy(buf, _pBuf, _ucLen);
	crc = CRC16_Modbus(_pBuf, _ucLen);
	buf[_ucLen++] = crc >> 8;
	buf[_ucLen++] = crc;
	
	HAL_UART_Transmit_IT(&huart3,buf,_ucLen);	//发送
  while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)!=SET);		//等待发送结束
	
#if 0									/* 此部分为了串口打印结果,实际运用中可不要 */
	g_tPrint.Txlen = _ucLen;
	memcpy(g_tPrint.TxBuf, buf, _ucLen);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckErr
*	功能说明: 发送错误应答
*	形    参: _ucErrCode : 错误代码
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendAckErr(uint8_t _ucErrCode)
{
	uint8_t txbuf[3];

	txbuf[0] = g_tModS.RxBuf[0];					/* 485地址 */
	txbuf[1] = g_tModS.RxBuf[1] | 0x80;				/* 异常的功能码 */
	txbuf[2] = _ucErrCode;							/* 错误代码(01,02,03,04) */

	MODS_SendWithCRC(txbuf, 3);
}

/*
*********************************************************************************************************
*	函 数 名: MODS_SendAckOk
*	功能说明: 发送正确的应答.
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_SendAckOk(void)
{
	uint8_t txbuf[6];
	uint8_t i;

	for (i = 0; i < 6; i++)
	{
		txbuf[i] = g_tModS.RxBuf[i];
	}
	MODS_SendWithCRC(txbuf, 6);
}

/*
*********************************************************************************************************
*	函 数 名: MODS_AnalyzeApp
*	功能说明: 分析应用层协议
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_AnalyzeApp(void)
{
	switch (g_tModS.RxBuf[1])				/* 第2个字节 功能码 */
	{
		case 0x01:							/* 读取线圈状态（此例程用led代替）*/
			MODS_01H();
			break;

//		case 0x02:							/* 读取输入状态（按键状态）*/
//			MODS_02H();
//			break;
		
		case 0x03:							/* 读取保持寄存器（此例程存在g_tVar中）*/
			MODS_03H();
			break;
		
		case 0x04:							/* 读取输入寄存器（ADC的值）*/
			MODS_04H();
			break;
		
		case 0x05:							/* 强制单线圈（设置led）*/
			MODS_05H();
			break;
		
		case 0x06:							/* 写单个保存寄存器（此例程改写g_tVar中的参数）*/
			MODS_06H();	
			break;
			
		case 0x10:							/* 写多个保存寄存器（此例程存在g_tVar中的参数）*/
			MODS_10H();
			break;
		
		default:
			g_tModS.RspCode = RSP_ERR_CMD;
			MODS_SendAckErr(g_tModS.RspCode);	/* 告诉主机命令错误 */
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_01H
*	功能说明: 读取线圈状态（对应远程开关D01/D02/D03）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
/* 说明:这里用LED代替继电器,便于观察现象 */
static void MODS_01H(void)
{
	/*
	 举例：
		主机发送:
			11 从机地址
			01 功能码
			00 寄存器起始地址高字节
			13 寄存器起始地址低字节
			00 寄存器数量高字节
			25 寄存器数量低字节
			0E CRC校验高字节
			84 CRC校验低字节

		从机应答: 	1代表ON，0代表OFF。若返回的线圈数不为8的倍数，则在最后数据字节未尾使用0代替. BIT0对应第1个
			11 从机地址
			01 功能码
			05 返回字节数
			CD 数据1(线圈0013H-线圈001AH)
			6B 数据2(线圈001BH-线圈0022H)
			B2 数据3(线圈0023H-线圈002AH)
			0E 数据4(线圈0032H-线圈002BH)
			1B 数据5(线圈0037H-线圈0033H)
			45 CRC校验高字节
			E6 CRC校验低字节

		例子:
			01 01 10 01 00 03   29 0B	--- 查询D01开始的3个继电器状态
			01 01 10 03 00 01   09 0A   --- 查询D03继电器的状态
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint16_t m;
	uint8_t status[10];
	g_tModS.RspCode = RSP_OK;

	/* 没有外部继电器，直接应答错误 */
	if(g_tModS.RxCount != 8)
	{		
		g_tModS.RspCode = RSP_ERR_VALUE;				/* 数据值域错误 */
		return;
		
	}
	
	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 			/* 寄存器号 */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);				/* 寄存器个数 */

	m = (num + 7) / 8;
	
	if ((reg >= REG_D01) && (num > 0) && (reg + num <= REG_DXX + 1))
	{
		for (i = 0; i < m; i++)
		{
			status[i] = 0;
		}
		for (i = 0; i < num; i++)
		{
			if (bsp_IsDoOn(i + 1 + reg - REG_D01))		/* 读LED的状态，写入状态寄存器的每一位 */
			{  
				status[i / 8] |= (1 << (i % 8));
			}
		}
	}
	else
	{
		g_tModS.RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
	}

	if (g_tModS.RspCode == RSP_OK)						/* 正确应答 */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = m;			/* 返回字节数 */

		for (i = 0; i < m; i++)
		{
			g_tModS.TxBuf[g_tModS.TxCount++] = status[i];	/* 继电器状态 */
		}
		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);				/* 告诉主机命令错误 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_02H
*	功能说明: 读取输入状态（对应K01～K03）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//static void MODS_02H(void)
//{
//	/*
//		主机发送:
//			11 从机地址
//			02 功能码
//			00 寄存器地址高字节
//			C4 寄存器地址低字节
//			00 寄存器数量高字节
//			16 寄存器数量低字节
//			BA CRC校验高字节
//			A9 CRC校验低字节

//		从机应答:  响应各离散输入寄存器状态，分别对应数据区中的每位值，1 代表ON；0 代表OFF。
//		           第一个数据字节的LSB(最低字节)为查询的寻址地址，其他输入口按顺序在该字节中由低字节
//		           向高字节排列，直到填充满8位。下一个字节中的8个输入位也是从低字节到高字节排列。
//		           若返回的输入位数不是8的倍数，则在最后的数据字节中的剩余位至该字节的最高位使用0填充。
//			11 从机地址
//			02 功能码
//			03 返回字节数
//			AC 数据1(00C4H-00CBH)
//			DB 数据2(00CCH-00D3H)
//			35 数据3(00D4H-00D9H)
//			20 CRC校验高字节
//			18 CRC校验低字节

//		例子:
//		01 02 20 01 00 08  23CC  ---- 读取T01-08的状态
//		01 02 20 04 00 02  B3CA  ---- 读取T04-05的状态
//		01 02 20 01 00 12  A207   ---- 读 T01-18
//	*/

//	uint16_t reg;
//	uint16_t num;
//	uint16_t i;
//	uint16_t m;
//	uint8_t status[10];

//	g_tModS.RspCode = RSP_OK;

//	if (g_tModS.RxCount != 8)
//	{
//		g_tModS.RspCode = RSP_ERR_VALUE;				/* 数据值域错误 */
//		return;
//	}

//	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 			/* 寄存器号 */
//	num = BEBufToUint16(&g_tModS.RxBuf[4]);				/* 寄存器个数 */

//	m = (num + 7) / 8;
//	if ((reg >= REG_T01) && (num > 0) && (reg + num <= REG_TXX + 1))
//	{
//		for (i = 0; i < m; i++)
//		{
//			status[i] = 0;
//		}
//		for (i = 0; i < num; i++)
//		{
//			if (bsp_GetKeyState((KEY_ID_E)(KID_K1 + reg - REG_T01 + i)))
//			{
//				status[i / 8] |= (1 << (i % 8));
//			}
//		}
//	}
//	else
//	{
//		g_tModS.RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
//	}

//	if (g_tModS.RspCode == RSP_OK)						/* 正确应答 */
//	{
//		g_tModS.TxCount = 0;
//		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
//		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
//		g_tModS.TxBuf[g_tModS.TxCount++] = m;			/* 返回字节数 */

//		for (i = 0; i < m; i++)
//		{
//			g_tModS.TxBuf[g_tModS.TxCount++] = status[i];	/* T01-02状态 */
//		}
//		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);
//	}
//	else
//	{
//		MODS_SendAckErr(g_tModS.RspCode);				/* 告诉主机命令错误 */
//	}
//}

/*
*********************************************************************************************************
*	函 数 名: MODS_ReadRegValue
*	功能说明: 读取保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 存放寄存器结果
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
	uint16_t value;
	
	switch (reg_addr)									/* 判断寄存器地址 */
	{
		case SLAVE_REG_P01:
			value =	((uint16_t*)(&DeviceA.rx_para.quality))[1];	
			break;

		case SLAVE_REG_P02:
			value =	((uint16_t*)(&DeviceA.rx_para.quality))[0];	
			break;
		
		case SLAVE_REG_P03:
			value =	((uint16_t*)(&DeviceB.rx_para.quality))[1];	
			break;

		case SLAVE_REG_P04:
			value =	((uint16_t*)(&DeviceB.rx_para.quality))[0];	
			break;
		
		case SLAVE_REG_P05:
			value =	((uint16_t*)(&DeviceC.rx_para.quality))[1];	
			break;

		case SLAVE_REG_P06:
			value =	((uint16_t*)(&DeviceC.rx_para.quality))[0];	
			break;
		
		case SLAVE_REG_P07:
			value =	((uint16_t*)(&DeviceD.rx_para.quality))[1];	
			break;

		case SLAVE_REG_P08:
			value =	((uint16_t*)(&DeviceD.rx_para.quality))[0];	
			break;
		
		case SLAVE_REG_P09:
			value =	((uint16_t*)(&DeviceE.rx_para.quality))[1];	
			break;

		case SLAVE_REG_P10:
			value =	((uint16_t*)(&DeviceE.rx_para.quality))[0];	
			break;

		case SLAVE_REG_P11:
			value =	*(&laserPWM.frequency);
			break;
		
		case SLAVE_REG_P12:
			value =	*(&laserPWM.duty);
			break;
		
		default:
			return 0;									/* 参数异常，返回 0 */
	}

	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* 读取成功 */
}

/*
*********************************************************************************************************
*	函 数 名: MODS_WriteRegValue
*	功能说明: 写保持寄存器的值
*	形    参: reg_addr 寄存器地址
*			  reg_value 寄存器值
*	返 回 值: 1表示OK 0表示错误
*********************************************************************************************************
*/
static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{
	static uint32_t tem_flow;
	switch (reg_addr)							/* 判断寄存器地址 */
	{	
		case SLAVE_REG_P01:
			tem_flow = (uint32_t)reg_value;
			break;
		
		case SLAVE_REG_P02:
			tem_flow = (tem_flow<<16|reg_value);
//			((unsigned char *)(&DeviceA.set_value_w))[0] = SPI_InByte();
//			((unsigned char *)(&DeviceA.set_value_w))[1] = SPI_InByte();
//			((unsigned char *)(&DeviceA.set_value_w))[2] = SPI_InByte();
//			((unsigned char *)(&DeviceA.set_value_w))[3] = SPI_InByte();
			DeviceA.set_value_w = write_hex_to_float(tem_flow);
			xEventGroupSetBits(flowGroupHandler,flow1_Device_Bit);
			break;
		
		case SLAVE_REG_P03:
			tem_flow = (uint32_t)reg_value;
			break;
		
		case SLAVE_REG_P04:
			tem_flow = (tem_flow<<16|reg_value);
			DeviceB.set_value_w = write_hex_to_float(tem_flow);
			xEventGroupSetBits(flowGroupHandler,flow2_Device_Bit);
			break;
		
		case SLAVE_REG_P05:
			tem_flow = (uint32_t)reg_value;
			break;
		
		case SLAVE_REG_P06:
			tem_flow = (tem_flow<<16|reg_value);
			DeviceC.set_value_w = write_hex_to_float(tem_flow);
			xEventGroupSetBits(flowGroupHandler,flow3_Device_Bit);
			break;
		
		case SLAVE_REG_P07:
			tem_flow = (uint32_t)reg_value;
			break;
		
		case SLAVE_REG_P08:
			tem_flow = (tem_flow<<16|reg_value);
			DeviceD.set_value_w = write_hex_to_float(tem_flow);
			xEventGroupSetBits(flowGroupHandler,flow4_Device_Bit);
			break;
		
		case SLAVE_REG_P09:
			tem_flow = (uint32_t)reg_value;
			break;
		
		case SLAVE_REG_P10:
			tem_flow = (tem_flow<<16|reg_value);
			DeviceE.set_value_w = write_hex_to_float(tem_flow);
			xEventGroupSetBits(flowGroupHandler,flow5_Device_Bit);
			break;
		
		case SLAVE_REG_P11:
			laserPWM.frequency = reg_value;
			xEventGroupSetBits(laserGroupHandler,laser_Frequency_Bit);
			break;
		
		case SLAVE_REG_P12:
			laserPWM.duty = reg_value;
			xEventGroupSetBits(laserGroupHandler,laser_Duty_Bit);
			break;
		
		default:
			return 0;		/* 参数异常，返回 0 */
	}

	return 1;		/* 读取成功 */
}

/*
*********************************************************************************************************
*	函 数 名: MODS_03H
*	功能说明: 读取保持寄存器 在一个或多个保持寄存器中取得当前的二进制值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_03H(void)
{
	/*
		从机地址为11H。保持寄存器的起始地址为006BH，结束地址为006DH。该次查询总共访问3个保持寄存器。

		主机发送:
			11 从机地址
			03 功能码
			00 寄存器地址高字节
			6B 寄存器地址低字节
			00 寄存器数量高字节
			03 寄存器数量低字节
			76 CRC高字节
			87 CRC低字节

		从机应答: 	保持寄存器的长度为2个字节。对于单个保持寄存器而言，寄存器高字节数据先被传输，
					低字节数据后被传输。保持寄存器之间，低地址寄存器先被传输，高地址寄存器后被传输。
			11 从机地址
			03 功能码
			06 字节数
			00 数据1高字节(006BH)
			6B 数据1低字节(006BH)
			00 数据2高字节(006CH)
			13 数据2 低字节(006CH)
			00 数据3高字节(006DH)
			00 数据3低字节(006DH)
			38 CRC高字节
			B9 CRC低字节

		例子:
			01 03 30 06 00 01  6B0B      ---- 读 3006H, 触发电流
			01 03 4000 0010 51C6         ---- 读 4000H 倒数第1条浪涌记录 32字节
			01 03 4001 0010 0006         ---- 读 4001H 倒数第1条浪涌记录 32字节

			01 03 F000 0008 770C         ---- 读 F000H 倒数第1条告警记录 16字节
			01 03 F001 0008 26CC         ---- 读 F001H 倒数第2条告警记录 16字节

			01 03 7000 0020 5ED2         ---- 读 7000H 倒数第1条波形记录第1段 64字节
			01 03 7001 0020 0F12         ---- 读 7001H 倒数第1条波形记录第2段 64字节

			01 03 7040 0020 5F06         ---- 读 7040H 倒数第2条波形记录第1段 64字节
	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[64];

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)								/* 03H命令必须是8个字节 */
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 				/* 寄存器号 */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);					/* 寄存器个数 */
	if (num > sizeof(reg_value) / 2)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;					/* 数据值域错误 */
		goto err_ret;
	}

	for (i = 0; i < num; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	/* 读出寄存器值放入reg_value */
		{
			g_tModS.RspCode = RSP_ERR_REG_ADDR;				/* 寄存器地址错误 */
			break;
		}
		reg++;
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)							/* 正确应答 */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = num * 2;			/* 返回字节数 */

		for (i = 0; i < num; i++)
		{
			g_tModS.TxBuf[g_tModS.TxCount++] = reg_value[2*i];
			g_tModS.TxBuf[g_tModS.TxCount++] = reg_value[2*i+1];
		}
		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);	/* 发送正确应答 */
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);					/* 发送错误应答 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_04H
*	功能说明: 读取输入寄存器（对应A01/A02） SMA
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_04H(void)
{
	/*
		主机发送:
			11 从机地址
			04 功能码
			00 寄存器起始地址高字节
			08 寄存器起始地址低字节
			00 寄存器个数高字节
			02 寄存器个数低字节
			F2 CRC高字节
			99 CRC低字节

		从机应答:  输入寄存器长度为2个字节。对于单个输入寄存器而言，寄存器高字节数据先被传输，
				低字节数据后被传输。输入寄存器之间，低地址寄存器先被传输，高地址寄存器后被传输。
			11 从机地址
			04 功能码
			04 字节数
			00 数据1高字节(0008H)
			0A 数据1低字节(0008H)
			00 数据2高字节(0009H)
			0B 数据2低字节(0009H)
			8B CRC高字节
			80 CRC低字节

		例子:

			01 04 2201 0006 2BB0  --- 读 2201H A01通道模拟量 开始的6个数据
			01 04 2201 0001 6A72  --- 读 2201H

	*/
	uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint16_t status[10];

	memset(status, 0, 10);

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;	/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	num = BEBufToUint16(&g_tModS.RxBuf[4]);	/* 寄存器个数 */
	
	if ((reg >= REG_A01) && (num > 0) && (reg + num <= REG_AXX + 1))
	{	
		for (i = 0; i < num; i++)
		{
			switch (reg)
			{
				/* 测试参数 */
				case REG_A01:
					status[i] = (uint16_t)(*(&per.signal));
					break;
				//暂未处理
				case REG_A02:
					status[i] = 0;
					break;
				case REG_A03:
					status[i] = 0;
					break;
				case REG_A04:
					status[i] = 0;
					break;
				case REG_A05:
					status[i] = 0;
					break;
				default:
					status[i] = 0;
					break;
			}
			reg++;
		}
	}
	else
	{
		g_tModS.RspCode = RSP_ERR_REG_ADDR;		/* 寄存器地址错误 */
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)		/* 正确应答 */
	{
		g_tModS.TxCount = 0;
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[0];
		g_tModS.TxBuf[g_tModS.TxCount++] = g_tModS.RxBuf[1];
		g_tModS.TxBuf[g_tModS.TxCount++] = num * 2;			/* 返回字节数 */

		for (i = 0; i < num; i++)
		{
			g_tModS.TxBuf[g_tModS.TxCount++] = status[i] >> 8;
			g_tModS.TxBuf[g_tModS.TxCount++] = status[i] & 0xFF;
		}
		MODS_SendWithCRC(g_tModS.TxBuf, g_tModS.TxCount);
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);	/* 告诉主机命令错误 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_05H
*	功能说明: 强制单线圈（对应D01/D02/D03）
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_05H(void)
{
	/*
		主机发送: 写单个线圈寄存器。FF00H值请求线圈处于ON状态，0000H值请求线圈处于OFF状态
		。05H指令设置单个线圈的状态，15H指令可以设置多个线圈的状态。
			11 从机地址
			05 功能码
			00 寄存器地址高字节
			AC 寄存器地址低字节
			FF 数据1高字节
			00 数据2低字节
			4E CRC校验高字节
			8B CRC校验低字节

		从机应答:
			11 从机地址
			05 功能码
			00 寄存器地址高字节
			AC 寄存器地址低字节
			FF 寄存器1高字节
			00 寄存器1低字节
			4E CRC校验高字节
			8B CRC校验低字节

		例子:
		01 05 10 01 FF 00   D93A   -- D01打开
		01 05 10 01 00 00   98CA   -- D01关闭

		01 05 10 02 FF 00   293A   -- D02打开
		01 05 10 02 00 00   68CA   -- D02关闭

		01 05 10 03 FF 00   78FA   -- D03打开
		01 05 10 03 00 00   390A   -- D03关闭
	*/
	uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;
	
	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	value = BEBufToUint16(&g_tModS.RxBuf[4]);	/* 数据 */
	
	if (value != 0 && value != 0xFF00)   // 修改
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* 数据值域错误 */
		goto err_ret;
	}
	
	if (reg == REG_D01)
	{
		if(value)
			bsp_DoOn(1);
		else
		  bsp_DoOff(1);
	}
	else if (reg == REG_D02)
	{
		if(value)
			bsp_DoOn(2);
		else
		  bsp_DoOff(2);	}
	else if (reg == REG_D03)
	{
		if(value)
			bsp_DoOn(3);
		else
		  bsp_DoOff(3);	}
	else if (reg == REG_D04)
	{
		if(value)
			bsp_DoOn(4);
		else
		  bsp_DoOff(4);	}
	else if (reg == REG_D05)
	{
		if(value)
			bsp_DoOn(5);
		else
		  bsp_DoOff(5);	}
	else if (reg == REG_D06)
	{
		if(value)
			bsp_DoOn(6);
		else
		  bsp_DoOff(6);	}
	else if (reg == REG_D07)
	{
		if(value)
			bsp_DoOn(7);
		else
		  bsp_DoOff(7);	}
	else if (reg == REG_D08)
	{
		if(value)
			bsp_DoOn(8);
		else
		  bsp_DoOff(8);	}
	else if (reg == REG_D09)
	{
		if(value)
			bsp_DoOn(9);
		else
		  bsp_DoOff(9);	}
	else if (reg == REG_D010)
	{
		if(value)
			bsp_DoOn(10);
		else
		  bsp_DoOff(10);	}
	else if (reg == REG_D11)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_sample_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_sample_end);
	}
	else if (reg == REG_D12)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_purge_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_purge_end);
	}
	else if (reg == REG_D13)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_OC_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_OC_end);
	}
	else if (reg == REG_D14)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_EC_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_EC_end);
	}
	else if (reg == REG_D15)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_ration_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_ration_end);
	}
	else if (reg == REG_D16)
	{
		if(value)
			xEventGroupSetBits(logicGroupHandler,logic_calibration_start);
		else
			xEventGroupSetBits(logicGroupHandler,logic_calibration_end);
	}
	else
	{
		g_tModS.RspCode = RSP_ERR_REG_ADDR;		/* 寄存器地址错误 */
	}
err_ret:
	if (g_tModS.RspCode == RSP_OK)				/* 正确应答 */
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);		/* 告诉主机命令错误 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_06H
*	功能说明: 写单个寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_06H(void)
{

	/*
		写保持寄存器。注意06指令只能操作单个保持寄存器，16指令可以设置单个或多个保持寄存器

		主机发送:
			11 从机地址
			06 功能码
			00 寄存器地址高字节
			01 寄存器地址低字节
			00 数据1高字节
			01 数据1低字节
			9A CRC校验高字节
			9B CRC校验低字节

		从机响应:
			11 从机地址
			06 功能码
			00 寄存器地址高字节
			01 寄存器地址低字节
			00 数据1高字节
			01 数据1低字节
			1B CRC校验高字节
			5A	CRC校验低字节

		例子:
			01 06 30 06 00 25  A710    ---- 触发电流设置为 2.5
			01 06 30 06 00 10  6707    ---- 触发电流设置为 1.0


			01 06 30 1B 00 00  F6CD    ---- SMA 滤波系数 = 0 关闭滤波
			01 06 30 1B 00 01  370D    ---- SMA 滤波系数 = 1
			01 06 30 1B 00 02  770C    ---- SMA 滤波系数 = 2
			01 06 30 1B 00 05  36CE    ---- SMA 滤波系数 = 5

			01 06 30 07 00 01  F6CB    ---- 测试模式修改为 T1
			01 06 30 07 00 02  B6CA    ---- 测试模式修改为 T2

			01 06 31 00 00 00  8736    ---- 擦除浪涌记录区
			01 06 31 01 00 00  D6F6    ---- 擦除告警记录区

*/

	uint16_t reg;
	uint16_t value;

	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount != 8)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;		/* 数据值域错误 */
		goto err_ret;
	}

	reg = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	value = BEBufToUint16(&g_tModS.RxBuf[4]);	/* 寄存器值 */

	if (MODS_WriteRegValue(reg, value) == 1)	/* 该函数会把写入的值存入寄存器 */
	{
		;
	}
	else
	{
		g_tModS.RspCode = RSP_ERR_REG_ADDR;		/* 寄存器地址错误 */
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)				/* 正确应答 */
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);		/* 告诉主机命令错误 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: MODS_10H
*	功能说明: 连续写多个寄存器.  进用于改写时钟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_10H(void)
{
	
	/*
		从机地址为11H。保持寄存器的其实地址为0001H，寄存器的结束地址为0002H。总共访问2个寄存器。
		保持寄存器0001H的内容为000AH，保持寄存器0002H的内容为0102H。

		主机发送:
			11 从机地址
			10 功能码
			00 寄存器起始地址高字节
			01 寄存器起始地址低字节
			00 寄存器数量高字节
			02 寄存器数量低字节
			04 字节数
			00 数据1高字节
			0A 数据1低字节
			01 数据2高字节
			02 数据2低字节
			C6 CRC校验高字节
			F0 CRC校验低字节

		从机响应:
			11 从机地址
			06 功能码
			00 寄存器地址高字节
			01 寄存器地址低字节
			00 数据1高字节
			01 数据1低字节
			1B CRC校验高字节
			5A	CRC校验低字节

		例子:
			01 10 30 00 00 06 0C  07 DE  00 0A  00 01  00 08  00 0C  00 00     389A    ---- 写时钟 2014-10-01 08:12:00
			01 10 30 00 00 06 0C  07 DF  00 01  00 1F  00 17  00 3B  00 39     5549    ---- 写时钟 2015-01-31 23:59:57

	*/
	uint16_t reg_addr;
	uint16_t reg_num;
	uint8_t byte_num;
	uint8_t i;
	uint16_t value;
	
	g_tModS.RspCode = RSP_OK;

	if (g_tModS.RxCount < 11)
	{
		g_tModS.RspCode = RSP_ERR_VALUE;			/* 数据值域错误 */
		goto err_ret;
	}

	reg_addr = BEBufToUint16(&g_tModS.RxBuf[2]); 	/* 寄存器号 */
	reg_num = BEBufToUint16(&g_tModS.RxBuf[4]);		/* 寄存器个数 */
	byte_num = g_tModS.RxBuf[6];					/* 后面的数据体字节数 */

	if (byte_num != 2 * reg_num)
	{
		;
	}
	
	for (i = 0; i < reg_num; i++)
	{
		value = BEBufToUint16(&g_tModS.RxBuf[7 + 2 * i]);	/* 寄存器值 */

		if (MODS_WriteRegValue(reg_addr + i, value) == 1)
		{
			;
		}
		else
		{
			g_tModS.RspCode = RSP_ERR_REG_ADDR;		/* 寄存器地址错误 */
			break;
		}
	}

err_ret:
	if (g_tModS.RspCode == RSP_OK)					/* 正确应答 */
	{
		MODS_SendAckOk();
	}
	else
	{
		MODS_SendAckErr(g_tModS.RspCode);			/* 告诉主机命令错误 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: CRC16_Modbus
*	功能说明: 计算CRC。 用于Modbus协议。
*	形    参: _pBuf : 参与校验的数据
*			  _usLen : 数据长度
*	返 回 值: 16位整数值。 对于Modbus ，此结果高字节先传送，低字节后传送。
*
*   所有可能的CRC值都被预装在两个数组当中，当计算报文内容时可以简单的索引即可；
*   一个数组包含有16位CRC域的所有256个可能的高位字节，另一个数组含有低位字节的值；
*   这种索引访问CRC的方式提供了比对报文缓冲区的每一个新字符都计算新的CRC更快的方法；
*
*  注意：此程序内部执行高/低CRC字节的交换。此函数返回的是已经经过交换的CRC值；也就是说，该函数的返回值可以直接放置
*        于报文用于发送；
*********************************************************************************************************
*/
uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen)
{
	uint8_t ucCRCHi = 0xFF; /* 高CRC字节初始化 */
	uint8_t ucCRCLo = 0xFF; /* 低CRC 字节初始化 */
	uint16_t usIndex;  /* CRC循环中的索引 */

    while (_usLen--)
    {
		usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
		ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
		ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}

/*
*********************************************************************************************************
*	函 数 名: BEBufToUint16
*	功能说明: 将2字节数组(大端Big Endian次序，高字节在前)转换为16位整数
*	形    参: _pBuf : 数组
*	返 回 值: 16位整数值
*
*   大端(Big Endian)与小端(Little Endian)
*********************************************************************************************************
*/
uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
