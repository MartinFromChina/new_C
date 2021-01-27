#include "terminal3.h"
#include "DG_common.h"

INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);
APP_DG_TERMINAL_DEFINE(p_entry,3,2,4);

X_Void main_loop_3(uint32_t current_time)
{
   INSERT(LogDebugCounter)(1,p_counter,100,("main3 still alive \r\n"));
	MainLoopHandle(p_entry,current_time);
}

X_Void UartIrqCallBack_3(uint8_t data)
{
   UartIrqHandle(p_entry,data);
}


