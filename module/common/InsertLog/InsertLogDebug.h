#ifndef __INSERT_LOG_DEBUG_H
#define __INSERT_LOG_DEBUG_H

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>
#include "../x_cross_platform.h"
#include "../CommonMarco.h"
#include <stdarg.h>

//typedef int (*log_func)(const char * sFormat,...);
typedef int (*log_func)(const char * sFormat, va_list param_list);

#define insert_LOG_ONCE_ENTRY_DEF(p_entry_name,init_value)            								\
				static uint16_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
				static uint16_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)

#define insert_LOG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            							\
				static uint32_t    CONCAT_2(p_entry_name,_whole) = init_value;						\
				static uint32_t*   p_entry_name = &CONCAT_2(p_entry_name,_whole)

#define remove_LOG_ONCE_ENTRY_DEF(p_entry_name,init_value)           
#define remove_LOG_COUNTER_ENTRY_DEF(p_entry_name,init_value)            					



int log_printf(const char * sFormat, ...);
#ifndef DEBUG_METHOD
#define DEBUG_METHOD  log_printf
#endif
X_Boolean OnceEntryCheck(uint16_t *backup_entry,uint16_t new_value) ;
X_Boolean CounterEntryCheck(uint32_t *backup_entry,uint32_t counter_limit);

#define insert_LogDebug(flag,message)   do{ 																\
												if(flag != 0)	\
												{	DEBUG_METHOD	message ;	   	}			\
											}while(0)
/****************print when entry == once_ref***************************/

#define insert_LogDebugOnce(flag,entry,once_ref,message)   do{ 																\
																	if(flag != 0 && OnceEntryCheck(entry,once_ref) == X_True)	\
																	{	DEBUG_METHOD	message ;	   	}			\
																}while(0)
/**************print when entry = 0 , entry ++ per call ,until entry == counter_ref ,then entry = 0***********************/
#define insert_LogDebugCounter(flag,entry,counter_ref,message)   do{ 																\
																		if(flag != 0 && CounterEntryCheck(entry,counter_ref) == X_True)	\
																		{	DEBUG_METHOD	message ;	   	}			\
																	 }while(0)

/*******************************************/
#define remove_LogDebug(flag,message)   do{ }while(0)
#define remove_LogDebugOnce(flag,entry,once_ref,message)   do{ }while(0)
#define remove_LogDebugCounter(flag,entry,counter_ref,message)   do{ }while(0)
/*******************************************/
X_Void insert_OnceEntryValueSet(uint16_t * p_entry,uint16_t value);
X_Void insert_CounterEntryValueSet(uint32_t * p_entry,uint32_t value);														
X_Void insert_LogDebugInit(X_Boolean  (*Log_method_init)(log_func *pp_func_reg));
X_Void insert_LogDebugModuleEnable(X_Void);
X_Void insert_LogDebugModuleDisable(X_Void);
X_Boolean insert_DoesLogDebugModuleEnable(X_Void);

X_Void remove_OnceEntryValueSet(uint16_t * p_entry,uint16_t value);
X_Void remove_CounterEntryValueSet(uint32_t * p_entry,uint32_t value);														
X_Void remove_LogDebugInit(X_Boolean  (*Log_method_init)(log_func *pp_func_reg));
X_Void remove_LogDebugModuleEnable(X_Void);
X_Void remove_LogDebugModuleDisable(X_Void);
X_Boolean remove_DoesLogDebugModuleEnable(X_Void);

/*******************************************/
#ifdef __cplusplus
	}
#endif

#endif

