#include <stdio.h>
#include "finally.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

#if (USE_INSERT_DEBUG != 0)
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,1);
INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);
#endif


static uint8_t i = 0;
uint8_t TestFunction(X_Void)
{
    i = i + 1;
	INSERT(LogDebugCounter)(1,p_counter,100,("hello\r\n")); 
	return i;
}

uint8_t GetX_Xita(X_Void)
{
	return (i%360);
}
uint8_t GetY_Xita(X_Void)
{
	return (i%360);
}
uint8_t GetZ_Xita(X_Void)
{
	return (i%360);
}



