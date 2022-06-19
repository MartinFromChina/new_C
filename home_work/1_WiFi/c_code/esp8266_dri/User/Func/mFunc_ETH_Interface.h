#ifndef __MFUNC_ETH_INTERFACE_H
#define __MFUNC_ETH_INTERFACE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"
#include "MCU_Board_Speci.h"
#define MAX_ETH_RECEIVE_CHAR_LENGTH            255

#define MAX_ETH_SEND_FRAME_TO_CACHE            5
#define NORMAL_MAX_ETH_FRAME_SEND_LENGTH       180

#if ((NORMAL_MAX_ETH_FRAME_SEND_LENGTH + 2) > UART_WIFI_MAX_TX_LENGTH)
	#error [ ETH_FRAME_SEND_LENGTH beyond scope !!!]
#endif
		
#define HUGE_ETH_FRAME_SEND_LENGTH             1022
#if ((HUGE_ETH_FRAME_SEND_LENGTH + 2) > UART_WIFI_MAX_TX_LENGTH)
	#error [ HUGE_ETH_FRAME_SEND_LENGTH beyond scope !!!]
#endif
		
#if (NORMAL_MAX_ETH_FRAME_SEND_LENGTH > HUGE_ETH_FRAME_SEND_LENGTH)
	#error [ HUGE_ETH_FRAME_SEND_LENGTH must be bigger than NORMAL_MAX_ETH_FRAME_SEND_LENGTH]
#endif		
		
#define HUGE_BUF_TOTAL_SIZE         4232

X_Void mFun_ETH_Init(X_Void);
X_Void mFun_ETH_LoopReceive(X_Void);
X_Boolean mFun_ETH_DoesNewCommandCome(uint8_t **pp_data,uint16_t *p_length);
X_Boolean mFunc_ETH_GetByte(uint8_t *p_data);
X_Boolean mFun_ETH_SendBuf(const uint8_t *p_data,uint16_t length);
X_Void mFun_ETH_ReceiveBufClear(X_Void);
X_Void mFunc_ETH_SendRequestInit(X_Void);
X_Void mFunc_ETH_SendRequestEnable(X_Void);
X_Void mFunc_ETH_SendRequestDisable(X_Void);
X_Boolean mFun_ETH_DoesSend(uint8_t **pp_data,uint16_t *p_length);
X_Boolean mFun_ETH_DoesHugeBufSend(uint8_t **pp_data,uint16_t *p_length);
X_Boolean mFun_ETH_SendBufRequest(const uint8_t *p_data,uint16_t length);
X_Boolean mFun_ETH_SendHugeBufRequest(uint8_t *p_data,uint16_t length);
X_Void mFun_ETH_ReleaseCurSendBuf(X_Void);
X_Void mFunc_ETH_SendAT(char *_Cmd);
X_Void mFun_ETH_LogDebug(const char * sFormat, ...);

X_Void mFunc_ETH_SpecificReceiveProcess(X_Void);
X_Boolean mFunc_ETH_DoesResponseGet(X_Void);
X_Boolean mFunc_ETH_WaitResponseEventRegister(char *p_string_to_wait);
X_Boolean mFunc_ETH_DoesRecvSendRequest(X_Void);

#ifdef __cplusplus
		}
#endif

#endif
