/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.c
*	版    本 : V2.0
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期       作者    	说明
*		V1.0    2013-02-01 armfly	正式发布
*		V1.1    2013-06-09 armfly 	FiFo结构增加TxCount成员变量，方便判断缓冲区满; 增加 清FiFo的函数
*		V1.2	2014-09-29 armfly	增加RS485 MODBUS接口。接收到新字节后，直接执行回调函数。
*		V1.3	2015-07-23 armfly  	增加 UART_T 结构的读写指针几个成员变量必须增加 __IO 修饰,否则优化后
*								会导致串口发送函数死机。
*		V1.4	2015-08-04 armfly  	解决UART4配置bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  	增加修改波特率的接口函数
*		V1.6	2018-09-07 armfly  	移植到STM32H7平台
*		V1.7	2018-10-01 armfly  	增加 Sending 标志，表示正在发送中
*		V1.8	2018-11-26 armfly  	增加UART8，第8个串口
*       V1.9	2020-04-06 eric2013 修改标志清除错误
*		V2.0	2020-08-12 XMDNDL	移植至STM32L476和F107使用
*
*	Copyright (C), 2020-2022, XMDNDL
*
*********************************************************************************************************
*/

#include "bsp_uart_fifo.h"
#include "LogDebugSwitch.h"
#include "bsp.h"
#include "usart.h"
#include "../../../../../../module/common/AppCommon.h"


#define UART_TRANS_TIME_THRESHOLD_IN_MS    1000

///* 定义每个串口结构体变量 */
#if UART1_FIFO_EN == 1
    static UART_T g_tUart1;
		uint8_t temp_uart1_rec_buf[1];
		static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* 接收缓冲区 */
		static uint8_t g_TxBuf1[UART_COM1_MAX_LENGTH];
		
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	uint8_t temp_uart2_rec_buf[1];
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* 接收缓冲区 */
	static uint8_t g_TxBuf2[UART_COM2_MAX_LENGTH];
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	uint8_t temp_uart3_rec_buf[1];
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* 接收缓冲区 */
	static uint8_t g_TxBuf3[UART_COM3_MAX_LENGTH];
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	uint8_t temp_uart4_rec_buf[1];
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* 接收缓冲区 */
	static uint8_t g_TxBuf4[UART_COM4_MAX_LENGTH];
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	uint8_t temp_uart5_rec_buf[1];
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* 接收缓冲区 */
	static uint8_t g_TxBuf5[UART_COM5_MAX_LENGTH];
#endif

///* 函数声明 */
static void UartSend(UART_T *_pUart,const uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);

///*
//*********************************************************************************************************
//*	函 数 名: ComToUart
//*	功能说明: 将COM端口号转换为UART指针
//*	形    参: _ucPort: 端口号(COM1 - COM5)
//*	返 回 值: uart指针
//*********************************************************************************************************
//*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	switch(_ucPort)
	{
		#if (UART1_FIFO_EN != 0)
		case COM1:
			return &g_tUart1;
		#endif
		#if (UART2_FIFO_EN != 0)
		case COM2:
			return &g_tUart2;
		#endif
		#if (UART3_FIFO_EN != 0)
		case COM3:
			return &g_tUart3;
		#endif
		#if (UART4_FIFO_EN != 0)
		case COM4:
			return &g_tUart4;
		#endif
		#if (UART5_FIFO_EN != 0)
		case COM5:
			return &g_tUart5;
		#endif
		default:
			break;
	}
	return 0;
}
///*
//*********************************************************************************************************
//*	函 数 名: comSendBuf
//*	功能说明: 向串口发送一组数据。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
//*	形    参: _ucPort: 端口号(COM1 - COM5)
//*			  _ucaBuf: 待发送的数据缓冲区
//*			  _usLen : 数据长度
//*	返 回 值: 无
//*********************************************************************************************************
//*/
void comSendBuf(COM_PORT_E _ucPort,const uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0){return;}
	UartSend(pUart, _ucaBuf, _usLen);
}

///*
//*********************************************************************************************************
//*	函 数 名: comSendChar
//*	功能说明: 向串口发送1个字节。数据放到发送缓冲区后立即返回，由中断服务程序在后台完成发送
//*	形    参: _ucPort: 端口号(COM1 - COM5)
//*			  _ucByte: 待发送的数据
//*	返 回 值: 无
//*********************************************************************************************************
//*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

///*
//*********************************************************************************************************
//*	函 数 名: comGetChar
//*	功能说明: 从串口缓冲区读取1字节，非阻塞。无论有无数据均立即返回
//*	形    参: _ucPort: 端口号(COM1 - COM5)
//*			  _pByte: 接收到的数据存放在这个地址
//*	返 回 值: 0 表示无数据, 1 表示读取到有效字节
//*********************************************************************************************************
//*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0){return 0;}
	return UartGetChar(pUart, _pByte);
}

///*
//*********************************************************************************************************
//*	函 数 名: comClearRxFifo
//*	功能说明: 清零串口接收缓冲区
//*	形    参: _ucPort: 端口号(COM1 - COM5)
//*	返 回 值: 无
//*********************************************************************************************************
//*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0){return;}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}
//*********************************************************************************************************
//*	函 数 名: UartVarInit
//*	功能说明: 初始化串口相关的变量
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = &huart1;						/* STM32 串口设备 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* 发送缓冲区指针 */
	g_tUart1.pRxBuf = g_RxBuf1;					/* 接收缓冲区指针 */
	g_tUart1.usTxBufSize = UART_COM1_MAX_LENGTH;	/* 发送缓冲区大小 */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart1.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart1.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart1.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart1.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart1.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart1.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart1.Sending = 0;						/* 正在发送中标志 */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = &huart2;						/* STM32 串口设备 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* 发送缓冲区指针 */
	g_tUart2.pRxBuf = g_RxBuf2;					/* 接收缓冲区指针 */
	g_tUart2.usTxBufSize = UART_COM2_MAX_LENGTH;	/* 发送缓冲区大小 */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart2.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart2.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart2.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart2.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart2.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart2.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart2.Sending = 0;						/* 正在发送中标志 */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = &huart3;						/* STM32 串口设备 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* 发送缓冲区指针 */
	g_tUart3.pRxBuf = g_RxBuf3;					/* 接收缓冲区指针 */
	g_tUart3.usTxBufSize = UART_COM3_MAX_LENGTH;	/* 发送缓冲区大小 */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart3.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart3.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart3.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart3.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart3.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart3.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart3.Sending = 0;						/* 正在发送中标志 */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = &huart4;						/* STM32 串口设备 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* 发送缓冲区指针 */
	g_tUart4.pRxBuf = g_RxBuf4;					/* 接收缓冲区指针 */
	g_tUart4.usTxBufSize = UART_COM4_MAX_LENGTH;	/* 发送缓冲区大小 */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart4.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart4.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart4.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart4.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart4.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart4.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart4.Sending = 0;						/* 正在发送中标志 */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = &huart5;						/* STM32 串口设备 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* 发送缓冲区指针 */
	g_tUart5.pRxBuf = g_RxBuf5;					/* 接收缓冲区指针 */
	g_tUart5.usTxBufSize = UART_COM5_MAX_LENGTH;	/* 发送缓冲区大小 */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* 接收缓冲区大小 */
	g_tUart5.usRxWrite = 0;						/* 接收FIFO写索引 */
	g_tUart5.usRxRead = 0;						/* 接收FIFO读索引 */
	g_tUart5.usRxCount = 0;						/* 接收到的新数据个数 */
	g_tUart5.SendBefor = 0;						/* 发送数据前的回调函数 */
	g_tUart5.SendOver = 0;						/* 发送完毕后的回调函数 */
	g_tUart5.ReciveNew = 0;						/* 接收到新数据后的回调函数 */
	g_tUart5.Sending = 0;						/* 正在发送中标志 */
#endif
}
static void UartSend(UART_T *_pUart,const uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint32_t start_cnt ;
	#if (USE_INSERT_DEBUG != 0)
	HAL_StatusTypeDef status;
	#endif
	if(_pUart ->usTxBufSize < _usLen)
	{
		INSERT(LogDebug)(UART_SEND_DEBUG,("UartSend _usLen %d > _pUart ->usTxBufSize \r\n",_usLen,_pUart ->usTxBufSize));
		return ;
	}
	if(_pUart ->pTxBuf == X_Null)
	{
		INSERT(LogDebug)(UART_SEND_DEBUG,("UartSend error \r\n"));
		return ;
	}
	
	start_cnt = HAL_GetTick();
	while(_pUart ->uart ->gState != HAL_UART_STATE_READY)
	{
		if((HAL_GetTick() - start_cnt  ) > UART_TRANS_TIME_THRESHOLD_IN_MS) 
		{
			INSERT(LogDebug)(UART_SEND_DEBUG,("UartSend timeout; gstate = %d\r\n",_pUart ->uart ->gState));
			return ;
		}
		
	}
	CopyBuffer(_ucaBuf,_pUart ->pTxBuf,_usLen);
	#if (USE_INSERT_DEBUG != 0)
	status = 
	#endif
	HAL_UART_Transmit_IT(_pUart ->uart,_pUart ->pTxBuf,_usLen);
	INSERT(LogDebug)(UART_SEND_DEBUG,(" %2x: sent %d bytes ; status = %d\r\n",_pUart ->uart,_usLen,status));
}
///*
//*********************************************************************************************************
//*	函 数 名: UartGetChar
//*	功能说明: 从串口接收缓冲区读取1字节数据 （用于主程序调用）
//*	形    参: _pUart : 串口设备
//*			  _pByte : 存放读取数据的指针
//*	返 回 值: 0 表示无数据  1表示读取到数据
//*********************************************************************************************************
//*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite 变量在中断函数中被改写，主程序读取该变量时，必须进行临界区保护 */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	//INSERT(LogDebug)(1,("-- %2x  %d\r\n",_pUart->uart,usCount));
	ENABLE_INT();

	/* 如果读和写索引相同，则返回0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* 已经没有数据 */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* 从串口接收FIFO取1个数据 */

		/* 改写FIFO读索引 */
		DISABLE_INT();
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
		ENABLE_INT();
		return 1;
	}
}

///*
//*********************************************************************************************************
//*   函 数 名: UartTxEmpty
//*   功能说明: 判断发送缓冲区是否为空。
//*   形    参:  _pUart : 串口设备
//*   返 回 值: 1为空。0为不空。
//*********************************************************************************************************
//*/
uint8_t comTxEmpty(COM_PORT_E _ucPort)
{
   return 1;
}

///*
//*********************************************************************************************************
//*	函 数 名: UartIRQ
//*	功能说明: 供中断服务程序调用，通用串口中断处理函数
//*	形    参: _pUart : 串口设备
//*	返 回 值: 无
//*********************************************************************************************************
//*/
static void UartIRQ(UART_T *_pUart,uint8_t data)
{
		/* 从串口接收数据寄存器读取数据存放到接收FIFO */
//		INSERT(LogDebug)(1,("%2x\r\n",ch));
		_pUart->pRxBuf[_pUart->usRxWrite] = data;
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
//			INSERT(LogDebug)(1,("_pUart->usRxCount = %d data %2x\r\n",_pUart->usRxCount,data));
		}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	X_Boolean isValid = X_False;
	UART_T * p_g_tUart;
	uint8_t  cur_data = 0;
  /* Prevent unused argument(s) compilation warning */
//	INSERT(LogDebug)(SYS_INIT_DEBUG,("HAL_UART_RxCpltCallback\r\n"));
	
		#if UART1_FIFO_EN == 1
		if(huart == &huart1)
		{
			HAL_UART_Receive_IT(huart,temp_uart1_rec_buf,1);
			isValid = X_True;
			p_g_tUart = &g_tUart1;
			cur_data = temp_uart1_rec_buf[0];
			INSERT(LogDebug)(UART_RECV_DEBUG,(" 1 rec %2x \r\n",cur_data));
		}
		#endif
		#if UART2_FIFO_EN == 1
		if(huart == &huart2)
		{
			HAL_UART_Receive_IT(huart,temp_uart2_rec_buf,1);
			isValid = X_True;
			p_g_tUart = &g_tUart2;
			cur_data = temp_uart2_rec_buf[0];
			INSERT(LogDebug)(UART_RECV_DEBUG,(" 2 rec %2x \r\n",cur_data));
		}
		#endif
		#if UART3_FIFO_EN == 1
		if(huart == &huart3)
		{
			HAL_UART_Receive_IT(huart,temp_uart3_rec_buf,1);
			isValid = X_True;
			p_g_tUart = &g_tUart3;
			cur_data = temp_uart3_rec_buf[0];
			INSERT(LogDebug)(UART_RECV_DEBUG,(" 3 rec %2x \r\n",cur_data));
		}
		#endif
		#if UART4_FIFO_EN == 1
		if(huart == &huart4)
		{
			HAL_UART_Receive_IT(huart,temp_uart4_rec_buf,1);
			isValid = X_True;
			p_g_tUart = &g_tUart4;
			cur_data = temp_uart4_rec_buf[0];
			INSERT(LogDebug)(UART_RECV_DEBUG ,(" 4 rec %2x \r\n",cur_data));
		}
		#endif
		#if UART5_FIFO_EN == 1
		if(huart == &huart5)
		{
			HAL_UART_Receive_IT(huart,temp_uart5_rec_buf,1);
			isValid = X_True;
			p_g_tUart = &g_tUart5;
			cur_data = temp_uart5_rec_buf[0];
		}
		#endif
		if(isValid == X_True)
		{
			UartIRQ(p_g_tUart,cur_data);
		}
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
////////	INSERT(LogDebug)(1,("  HAL_UART_ErrorCallback  ... "));
	__HAL_UNLOCK(huart);
	#if UART1_FIFO_EN == 1
		
		if(huart == &huart1)
		{
			HAL_UART_Receive_IT(huart,temp_uart1_rec_buf,1);
			INSERT(LogDebug)(1,(" 1  HAL_UART_ErrorCallback\r\n"));
		}
		#endif
		#if UART2_FIFO_EN == 1
		if(huart == &huart2)
		{
			HAL_UART_Receive_IT(huart,temp_uart2_rec_buf,1);
			INSERT(LogDebug)(0,(" 2  HAL_UART_ErrorCallback\r\n")); //
		}
		#endif
		#if UART3_FIFO_EN == 1
		if(huart == &huart3)
		{
			HAL_UART_Receive_IT(huart,temp_uart3_rec_buf,1);
			
			INSERT(LogDebug)(0,(" 3  HAL_UART_ErrorCallback\r\n"));
		}
		#endif
		#if UART4_FIFO_EN == 1
		if(huart == &huart4)
		{
			HAL_UART_Receive_IT(huart,temp_uart4_rec_buf,1);
			
			INSERT(LogDebug)(1,(" 4  HAL_UART_ErrorCallback\r\n"));
		}
		#endif
		#if UART5_FIFO_EN == 1
		if(huart == &huart5)
		{
			HAL_UART_Receive_IT(huart,temp_uart5_rec_buf,1);
			INSERT(LogDebug)(1,(" 5  HAL_UART_ErrorCallback\r\n"));
		}
		#endif
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_ErrorCallback could be implemented in the user file
   */
}
/***************************** XMDNDL (END OF FILE) *********************************/
