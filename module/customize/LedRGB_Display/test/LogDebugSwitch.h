#ifndef __LOGDEBUGSWITCH_H
#define __LOGDEBUGSWITCH_H

#ifdef __cplusplus
	extern "C"{
#endif
#include "../../../common/InsertLog/InsertLogDebug.h"

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

	#define STATE_DEBUG         1
	#define ON_OFF_DEBUG		1
	extern uint8_t mockable_GetDebugFlag(X_Void);
	extern uint32_t mockable_GetCurrentTime(X_Void);
#ifdef __cplusplus
		}
#endif
#endif