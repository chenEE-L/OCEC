/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#define RXBUFFERSIZE1   1 					//�����С
#define RXBUFFERSIZE2  	1 					//�����С
#define RXBUFFERSIZE3   1 					//�����С
#define RXBUFFERSIZE4   1 					//�����С
#define RXBUFFERSIZE5   1 					//�����С



/* USER CODE END Includes */

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
extern uint8_t aRx1Buffer[RXBUFFERSIZE1];//HAL��ʹ�õĴ��ڽ��ջ���
extern uint8_t aRx2Buffer[RXBUFFERSIZE2];//HAL��ʹ�õĴ��ڽ��ջ���
extern uint8_t aRx3Buffer[RXBUFFERSIZE3];//HAL��ʹ�õĴ��ڽ��ջ���
extern uint8_t aRx4Buffer[RXBUFFERSIZE4];//HAL��ʹ�õĴ��ڽ��ջ���
extern uint8_t aRx5Buffer[RXBUFFERSIZE5];//HAL��ʹ�õĴ��ڽ��ջ���

extern xQueueHandle COM1DataDelivery;		/*USB���⴮�����ݽ��ն���*/
extern xQueueHandle COM2DataDelivery;		/*USB���⴮�����ݽ��ն���*/
extern xQueueHandle COM3DataDelivery;		/*USB���⴮�����ݽ��ն���*/
extern xQueueHandle COM4DataDelivery;		/*USB���⴮�����ݽ��ն���*/
extern xQueueHandle COM5DataDelivery;		/*USB���⴮�����ݽ��ն���*/


/* USER CODE END Private defines */

void MX_UART4_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
 int COMGetDataWithTimout(xQueueHandle comQueue,uint8_t *c);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

