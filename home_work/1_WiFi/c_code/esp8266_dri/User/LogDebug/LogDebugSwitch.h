#ifndef __LOGDEBUGSWITCH_H
#define __LOGDEBUGSWITCH_H

#ifdef __cplusplus
	extern "C"{
#endif

#ifndef USE_TDD_MOCK
	#include "ProjectConfig.h"
#endif

/*****************************SYS_INIT*************************************/
#define LOG_DEBUG_METHOD 	SeggerRTT_LogDebug
#define USE_INSERT_DEBUG		1
		
#if (USE_DEBUG_MODE == 0 && USE_INSERT_DEBUG != 0)
		#error [cannot use INSERT_LOG debug in real product !!!!]
#endif

#if (USE_DEBUG_MODE == 0 && USE_SEGGER_RTT_READ_COMMAND != 0)
		#error [cannot USE_SEGGER_RTT_READ_COMMAND debug in real product !!!!]
#endif		
		
/******************************************************************************/




/******************************************************************************/
#define	USE_SEGGER_RTT_READ_COMMAND    1
/******************************************************************************/
#include "../../../../../../module/common/InsertLog/InsertLogDebug.h"
/******************************************************************************/

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

#if (USE_TDD_MOCK != 0)
		#define MOCKABLE(method)  mockable_##method
	#else
		#define MOCKABLE(method)  method
	#endif
		
X_Void LogDebugInit(X_Void);
X_Void SeggerRTT_LogDebug(const char * sFormat, ...);
/*****************************SYS_INIT*************************************/
#define SYS_INIT_DEBUG				1
#define SYS_INFO_DEBUG        1

/**********************************************************************/
#define BUTTON_DOING_DEBUG    1
#define UseButtonBehaviorAnalyze  1
/**********************************************************************/
#define UART_SEND_DEBUG           0
#define UART_RECV_DEBUG           0
/**********************************************************************/
#define TEST_MODE_DATA_REC_DEBUG  0
#define ETH_MODULE_DEBUG          1
#define ETH_DATA_DEBUG            1
#define ETH_DATA_RECV_DEBUG       1
#define ETH_SENDBUF_DEBUG         1
#define ETH_TRANS_DEBUG           1
#define ETH_RECEIVE_DEBUG         1
#define ETH_SPECIAL_RECEIVE_DEBUG  1
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/***********************************************************************
INSERT(LogDebug)(SYS_INIT_DEBUG,("\r\n---DGJC_System started successfully---\r\n")); 
**************************************************************************/
#if (USE_SEGGER_RTT_READ_COMMAND != 0)
	#define MAX_TM_FRAME_SEND_LENGTH   	200 // fix it later

	X_Void SeggerRttLoopRead(X_Void);

	X_Void mDri_TestModeReceiveBufClear(X_Void);
	X_Boolean mDri_TestModeGetByteInterface(uint8_t *p_data);
	X_Boolean mDri_TestModeSentBufInterface(const uint8_t *p_buf,uint16_t length);
	X_Boolean mDri_DoesTestModeSentBufEmpty(X_Void);
#endif


#include "SysLog.h"

#ifdef __cplusplus
		}
#endif
#endif
