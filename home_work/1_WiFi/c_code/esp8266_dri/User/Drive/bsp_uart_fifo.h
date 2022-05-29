/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
/*
	�Ϲ�MCU ���ڷ��䣺
	������1��	��������BC95ģ��
		PA9/USART1_TX
		P10/USART1_RX

	������2�� PD5/PD6����GPS���ӣ�PA2/PA3����shell
		PA2/USART2_TX	 
		PA3/USART2_RX

		PD5/USART2_TX	 	ʹ��PD5/PD6��������2ʱ����Ҫ��InitHardUart() �����ڿ�����ӳ�� 
		PD6/USART2_RX		GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);

	������3�� ��������ETH/WIFIģ��
		PB10/USART3_TX	 
		PB11/USART3_RX

	������4�� ����������չ���䵱Shell����̨ʹ��
		PC10/USART4_TX	 
		PC11/USART4_RX
	
	������5�� --- ��δʹ��
		PC12/USART5_TX	 
		PD2/USART5_RX
*/
#include "LogDebugSwitch.h"
#include "stm32f1xx_hal.h"
#include "MCU_Board_Speci.h"
///////////* RS485оƬ����ʹ��GPIO, PB9 */
//////////#define RCC_RS485_TXEN 	 RCC_APB2Periph_GPIOB
//////////#define PORT_RS485_TXEN  GPIOB
//////////#define PIN_RS485_TXEN	 GPIO_Pin_9

//////////#define RS485_RX_EN()	PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
//////////#define RS485_TX_EN()	PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN


/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PD5, PD6 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
	#define UART1_RX_BUF_SIZE	1*1024
#endif

#if UART2_FIFO_EN == 1
	#define UART2_RX_BUF_SIZE	1*1024
#endif
#if UART3_FIFO_EN == 1
	#define UART3_RX_BUF_SIZE	1*1024
#endif

#if UART4_FIFO_EN == 1
	#define UART4_RX_BUF_SIZE	1*1024
#endif

#if UART5_FIFO_EN == 1
	#define UART5_RX_BUF_SIZE	1*1024
#endif



///* �����豸�ṹ�� */
typedef struct
{
	UART_HandleTypeDef *uart;				/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;					/* ���ͻ����� */
	uint8_t *pRxBuf;					/* ���ջ����� */
	uint16_t usTxBufSize;				/* ���ͻ�������С */
	uint16_t usRxBufSize;				/* ���ջ�������С */

	__IO uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;				/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 			/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 			/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
	uint8_t Sending;					/* ���ڷ����� */
}UART_T;

/* �������ⲿ���õ�API */
void UartVarInit(void);
void comSendBuf(COM_PORT_E _ucPort, const uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearRxFifo(COM_PORT_E _ucPort);
uint8_t comTxEmpty(COM_PORT_E _ucPort);
#endif

/***************************** XMDNDL(END OF FILE) *********************************/
