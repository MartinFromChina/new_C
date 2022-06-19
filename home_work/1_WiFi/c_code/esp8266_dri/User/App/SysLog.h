#ifndef __SYSLOG_H
#define __SYSLOG_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "MCU_Board_Speci.h"
/******************************************************************************/
#include "../../../../../module/common/InsertLog/InsertLogDebug.h"
/******************************************************************************/		
	/*
		Do not call it in a ISR 
		the eth data port and printf log func use the same uart ; not a good idea!!!
	*/
////		#define SYS_LOG_ONCE_ENTRY_DEF(p_entry_name,init_value)            								
////		#define SYS_LOG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            						
////					
////		#define SysLogDebug(flag,message)       												do{ }while(0)
////		#define SysLogDebugOnce(flag,entry,once_ref,message)   					do{ }while(0)
////		#define SysLogDebugCounter(flag,entry,counter_ref,message)  		do{ }while(0)
#include "mFunc_TM_Interface.h"
#include  "mModule.h"

#define SYS_DEBUG_METHOD 				TM_DEBUG_METHOD 
#define TESTMODE_DEBUG_METHOD 	TM_DEBUG_METHOD
																							 
		#define SYS_LOG_ONCE_ENTRY_DEF(p_entry_name,init_value)            								\
				static uint16_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
				static uint16_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)

		#define SYS_LOG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            							\
				static uint32_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
				static uint32_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)
					
		#define SysLogDebug(flag,message)       do{ 																\
																								if((flag) != 0)								\
																								{	\
																									if(mModule_DoesInTestMode() == X_False)	\
																									{	\
																										SYS_DEBUG_METHOD	message ;	\
																									}	\
																									else	\
																									{	\
																										TESTMODE_DEBUG_METHOD message;	\
																									}	\
																								}	\
																							}while(0)
		#define SysLogDebugOnce(flag,entry,once_ref,message)   do{ 																\
																								if((flag) != 0 && OnceEntryCheck(entry,once_ref) == X_True)	\
																								{	\
																									if(mModule_DoesInTestMode() == X_False)	\
																									{	\
																										SYS_DEBUG_METHOD	message ;	\
																									}	\
																									else	\
																									{	\
																										TESTMODE_DEBUG_METHOD message;	\
																									}	\
																								}	\
																							}while(0)
/**************print when entry = 0 , entry ++ per call ,until entry == counter_ref ,then entry = 0***********************/
		#define SysLogDebugCounter(flag,entry,counter_ref,message)   do{ 																\
																								if((flag) != 0 && CounterEntryCheck(entry,counter_ref) == X_True)	\
																								{	\
																									if(mModule_DoesInTestMode() == X_False)	\
																									{	\
																										SYS_DEBUG_METHOD	message ;	\
																									}	\
																									else	\
																									{	\
																										TESTMODE_DEBUG_METHOD message;	\
																									}	\
																								}	\
																							 }while(0)
/*****************************SYS_INIT***********************************/
/************************************************************************
SysLogDebug(SYS_INIT_DEBUG,("\r\n---DGJC_System started successfully---\r\n")); 
**************************************************************************/
#ifdef __cplusplus
		}
#endif
#endif
