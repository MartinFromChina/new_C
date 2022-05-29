#include "mDri_Uart.h"
#include "bsp.h"
#include "usart.h"
#include "bsp_uart_fifo.h"
#include "../../../../../../module/common/AppCommon.h"
#include "MCU_Board_Speci.h"

X_Boolean isInitOK = X_False;
X_Void mDri_UartInit(X_Void)
{
	UartVarInit();
	#if UART1_FIFO_EN == 1		/* 串口1 TX = PA9   RX = PA10 或 TX = PB6   RX = PB7*/
	MX_USART1_UART_Init();
	#endif

	#if UART2_FIFO_EN == 1		/* 串口2 (TX = PA2， RX = PA3） shell（PA2/PA3） */
		MX_USART2_UART_Init();
	#endif

	#if UART3_FIFO_EN == 1			/* 串口3 TX = PB10   RX = PB11 */
		MX_USART3_UART_Init();
	#endif

	#if UART4_FIFO_EN == 1			/* 串口4 TX = PC10   RX = PC11 */
		MX_UART4_Init();
	#endif

	#if UART5_FIFO_EN == 1			/* 串口5 TX = PC12   RX = PD2 */
		MX_UART5_Init();
	#endif
	isInitOK = X_True;
}

X_Boolean mDri_WifiGetByteInterface(uint8_t *p_data)
{
	if(isInitOK == X_False || p_data == X_Null){return X_False;}
	return (comGetChar(ETH_UART_PORT,p_data) == 1);
}

X_Boolean mDri_WifiSentBufInterface(const uint8_t *p_buf,uint16_t length)
{
	if(isInitOK == X_False || p_buf == X_Null){return X_False;}
	comSendBuf(ETH_UART_PORT,p_buf,length);
	return X_True;
}

X_Boolean mDri_DoesWifiSentBufEmpty(X_Void)
{
	return (comTxEmpty(ETH_UART_PORT) == 1);
}
X_Void mDri_WifiReceiveBufClear(X_Void)
{
	comClearRxFifo(ETH_UART_PORT);
}

