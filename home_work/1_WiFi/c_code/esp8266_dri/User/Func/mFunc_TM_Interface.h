#ifndef __MFUNC_TM_INTERFACE_H
#define __MFUNC_TM_INTERFACE_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "../LogDebug/LogDebugSwitch.h"

X_Void mFun_TM_Init(X_Void);
X_Void mFun_TM_ReceiveClear(X_Void);
X_Boolean mFun_TM_GetByte(uint8_t *p_data);
X_Boolean mFun_TM_SendBuf(const uint8_t *p_data,uint16_t length);
X_Void mFun_TM_LogDebug(const char * sFormat, ...);
X_Void mFun_TM_ReceiveLocked(X_Void);
X_Void mFun_TM_ReceiveUnocked(X_Void);
/*
		Do not call it in a ISR ; do the uart send in a isr is not a good idea
	*/
/******************************************************************************/
	#include "../../../../../../module/common/InsertLog/InsertLogDebug.h"
/******************************************************************************/
	#define TM_DEBUG_METHOD mFun_TM_LogDebug
	
	#define TM_LOG_ONCE_ENTRY_DEF(p_entry_name,init_value)            								\
			static uint16_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
			static uint16_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)

	#define TM_LOG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            							\
			static uint32_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
			static uint32_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)
				
	#define TM_LogDebug(flag,message)       do{ 																\
																							if((flag) != 0)								\
																							{	TM_DEBUG_METHOD	message ;	   	}			\
																						}while(0)
	#define TM_LogDebugOnce(flag,entry,once_ref,message)   do{ 																\
																							if((flag) != 0 && OnceEntryCheck(entry,once_ref) == X_True)	\
																							{	TM_DEBUG_METHOD	message ;	   	}			\
																						}while(0)
/**************print when entry = 0 , entry ++ per call ,until entry == counter_ref ,then entry = 0***********************/
	#define TM_LogDebugCounter(flag,entry,counter_ref,message)   do{ 																\
																							if((flag) != 0 && CounterEntryCheck(entry,counter_ref) == X_True)	\
																							{	TM_DEBUG_METHOD	message ;	   	}			\
																						 }while(0)

#ifdef __cplusplus
		}
#endif

#endif
