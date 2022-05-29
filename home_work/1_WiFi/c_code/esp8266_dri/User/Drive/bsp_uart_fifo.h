/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
/*
	断轨MCU 串口分配：
	【串口1】	用于连接BC95模组
		PA9/USART1_TX
		P10/USART1_RX

	【串口2】 PD5/PD6用于GPS连接，PA2/PA3用作shell
		PA2/USART2_TX	 
		PA3/USART2_RX

		PD5/USART2_TX	 	使用PD5/PD6用作串口2时，需要在InitHardUart() 函数内开启重映射 
		PD6/USART2_RX		GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);

	【串口3】 用于连接ETH/WIFI模块
		PB10/USART3_TX	 
		PB11/USART3_RX

	【串口4】 用于连接扩展板或充当Shell调试台使用
		PC10/USART4_TX	 
		PC11/USART4_RX
	
	【串口5】 --- 暂未使用
		PC12/USART5_TX	 
		PD2/USART5_RX
*/
#include "LogDebugSwitch.h"
#include "stm32f1xx_hal.h"
#include "MCU_Board_Speci.h"
///////////* RS485芯片发送使能GPIO, PB9 */
//////////#define RCC_RS485_TXEN 	 RCC_APB2Periph_GPIOB
//////////#define PORT_RS485_TXEN  GPIOB
//////////#define PIN_RS485_TXEN	 GPIO_Pin_9

//////////#define RS485_RX_EN()	PORT_RS485_TXEN->BRR = PIN_RS485_TXEN
//////////#define RS485_TX_EN()	PORT_RS485_TXEN->BSRR = PIN_RS485_TXEN


/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PD5, PD6 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
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



///* 串口设备结构体 */
typedef struct
{
	UART_HandleTypeDef *uart;				/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;					/* 发送缓冲区 */
	uint8_t *pRxBuf;					/* 接收缓冲区 */
	uint16_t usTxBufSize;				/* 发送缓冲区大小 */
	uint16_t usRxBufSize;				/* 接收缓冲区大小 */

	__IO uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;				/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;			/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 			/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 			/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
	uint8_t Sending;					/* 正在发送中 */
}UART_T;

/* 声明供外部调用的API */
void UartVarInit(void);
void comSendBuf(COM_PORT_E _ucPort, const uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearRxFifo(COM_PORT_E _ucPort);
uint8_t comTxEmpty(COM_PORT_E _ucPort);
#endif

/***************************** XMDNDL(END OF FILE) *********************************/
