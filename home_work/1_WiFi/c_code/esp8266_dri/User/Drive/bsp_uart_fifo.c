/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.c
*	��    �� : V2.0
*	˵    �� : ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    	˵��
*		V1.0    2013-02-01 armfly	��ʽ����
*		V1.1    2013-06-09 armfly 	FiFo�ṹ����TxCount��Ա�����������жϻ�������; ���� ��FiFo�ĺ���
*		V1.2	2014-09-29 armfly	����RS485 MODBUS�ӿڡ����յ����ֽں�ֱ��ִ�лص�������
*		V1.3	2015-07-23 armfly  	���� UART_T �ṹ�Ķ�дָ�뼸����Ա������������ __IO ����,�����Ż���
*								�ᵼ�´��ڷ��ͺ���������
*		V1.4	2015-08-04 armfly  	���UART4����bug  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART1);
*		V1.5	2015-10-08 armfly  	�����޸Ĳ����ʵĽӿں���
*		V1.6	2018-09-07 armfly  	��ֲ��STM32H7ƽ̨
*		V1.7	2018-10-01 armfly  	���� Sending ��־����ʾ���ڷ�����
*		V1.8	2018-11-26 armfly  	����UART8����8������
*       V1.9	2020-04-06 eric2013 �޸ı�־�������
*		V2.0	2020-08-12 XMDNDL	��ֲ��STM32L476��F107ʹ��
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

///* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
    static UART_T g_tUart1;
		uint8_t temp_uart1_rec_buf[1];
		static uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
		static uint8_t g_TxBuf1[UART_COM1_MAX_LENGTH];
		
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	uint8_t temp_uart2_rec_buf[1];
	static uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
	static uint8_t g_TxBuf2[UART_COM2_MAX_LENGTH];
#endif

#if UART3_FIFO_EN == 1
	static UART_T g_tUart3;
	uint8_t temp_uart3_rec_buf[1];
	static uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
	static uint8_t g_TxBuf3[UART_COM3_MAX_LENGTH];
#endif

#if UART4_FIFO_EN == 1
	static UART_T g_tUart4;
	uint8_t temp_uart4_rec_buf[1];
	static uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
	static uint8_t g_TxBuf4[UART_COM4_MAX_LENGTH];
#endif

#if UART5_FIFO_EN == 1
	static UART_T g_tUart5;
	uint8_t temp_uart5_rec_buf[1];
	static uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
	static uint8_t g_TxBuf5[UART_COM5_MAX_LENGTH];
#endif

///* �������� */
static void UartSend(UART_T *_pUart,const uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);

///*
//*********************************************************************************************************
//*	�� �� ��: ComToUart
//*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
//*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
//*	�� �� ֵ: uartָ��
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
//*	�� �� ��: comSendBuf
//*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
//*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
//*			  _ucaBuf: �����͵����ݻ�����
//*			  _usLen : ���ݳ���
//*	�� �� ֵ: ��
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
//*	�� �� ��: comSendChar
//*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
//*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
//*			  _ucByte: �����͵�����
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

///*
//*********************************************************************************************************
//*	�� �� ��: comGetChar
//*	����˵��: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
//*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
//*			  _pByte: ���յ������ݴ���������ַ
//*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
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
//*	�� �� ��: comClearRxFifo
//*	����˵��: ���㴮�ڽ��ջ�����
//*	��    ��: _ucPort: �˿ں�(COM1 - COM5)
//*	�� �� ֵ: ��
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
//*	�� �� ��: UartVarInit
//*	����˵��: ��ʼ��������صı���
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
void UartVarInit(void)
{
#if UART1_FIFO_EN == 1
	g_tUart1.uart = &huart1;						/* STM32 �����豸 */
	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
	g_tUart1.usTxBufSize = UART_COM1_MAX_LENGTH;	/* ���ͻ�������С */
	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart1.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart1.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart1.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
	g_tUart1.Sending = 0;						/* ���ڷ����б�־ */
#endif

#if UART2_FIFO_EN == 1
	g_tUart2.uart = &huart2;						/* STM32 �����豸 */
	g_tUart2.pTxBuf = g_TxBuf2;					/* ���ͻ�����ָ�� */
	g_tUart2.pRxBuf = g_RxBuf2;					/* ���ջ�����ָ�� */
	g_tUart2.usTxBufSize = UART_COM2_MAX_LENGTH;	/* ���ͻ�������С */
	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart2.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart2.usRxRead = 0;						/* ����FIFO������ */
	g_tUart2.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart2.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart2.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart2.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
	g_tUart2.Sending = 0;						/* ���ڷ����б�־ */
#endif

#if UART3_FIFO_EN == 1
	g_tUart3.uart = &huart3;						/* STM32 �����豸 */
	g_tUart3.pTxBuf = g_TxBuf3;					/* ���ͻ�����ָ�� */
	g_tUart3.pRxBuf = g_RxBuf3;					/* ���ջ�����ָ�� */
	g_tUart3.usTxBufSize = UART_COM3_MAX_LENGTH;	/* ���ͻ�������С */
	g_tUart3.usRxBufSize = UART3_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart3.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart3.usRxRead = 0;						/* ����FIFO������ */
	g_tUart3.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart3.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart3.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart3.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
	g_tUart3.Sending = 0;						/* ���ڷ����б�־ */
#endif

#if UART4_FIFO_EN == 1
	g_tUart4.uart = &huart4;						/* STM32 �����豸 */
	g_tUart4.pTxBuf = g_TxBuf4;					/* ���ͻ�����ָ�� */
	g_tUart4.pRxBuf = g_RxBuf4;					/* ���ջ�����ָ�� */
	g_tUart4.usTxBufSize = UART_COM4_MAX_LENGTH;	/* ���ͻ�������С */
	g_tUart4.usRxBufSize = UART4_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart4.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart4.usRxRead = 0;						/* ����FIFO������ */
	g_tUart4.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart4.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart4.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart4.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
	g_tUart4.Sending = 0;						/* ���ڷ����б�־ */
#endif

#if UART5_FIFO_EN == 1
	g_tUart5.uart = &huart5;						/* STM32 �����豸 */
	g_tUart5.pTxBuf = g_TxBuf5;					/* ���ͻ�����ָ�� */
	g_tUart5.pRxBuf = g_RxBuf5;					/* ���ջ�����ָ�� */
	g_tUart5.usTxBufSize = UART_COM5_MAX_LENGTH;	/* ���ͻ�������С */
	g_tUart5.usRxBufSize = UART5_RX_BUF_SIZE;	/* ���ջ�������С */
	g_tUart5.usRxWrite = 0;						/* ����FIFOд���� */
	g_tUart5.usRxRead = 0;						/* ����FIFO������ */
	g_tUart5.usRxCount = 0;						/* ���յ��������ݸ��� */
	g_tUart5.SendBefor = 0;						/* ��������ǰ�Ļص����� */
	g_tUart5.SendOver = 0;						/* ������Ϻ�Ļص����� */
	g_tUart5.ReciveNew = 0;						/* ���յ������ݺ�Ļص����� */
	g_tUart5.Sending = 0;						/* ���ڷ����б�־ */
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
//*	�� �� ��: UartGetChar
//*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
//*	��    ��: _pUart : �����豸
//*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
//*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
//*********************************************************************************************************
//*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	DISABLE_INT();
	usCount = _pUart->usRxCount;
	//INSERT(LogDebug)(1,("-- %2x  %d\r\n",_pUart->uart,usCount));
	ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
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
//*   �� �� ��: UartTxEmpty
//*   ����˵��: �жϷ��ͻ������Ƿ�Ϊ�ա�
//*   ��    ��:  _pUart : �����豸
//*   �� �� ֵ: 1Ϊ�ա�0Ϊ���ա�
//*********************************************************************************************************
//*/
uint8_t comTxEmpty(COM_PORT_E _ucPort)
{
   return 1;
}

///*
//*********************************************************************************************************
//*	�� �� ��: UartIRQ
//*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
//*	��    ��: _pUart : �����豸
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
static void UartIRQ(UART_T *_pUart,uint8_t data)
{
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
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
