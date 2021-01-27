#include "terminal2.h"
#include "DG_common.h"

INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);
APP_DG_TERMINAL_DEFINE(p_entry,2,1,3);

X_Void main_loop_2(uint32_t current_time)
{
   INSERT(LogDebugCounter)(1,p_counter,30,("main2 still alive \r\n"));
	MainLoopHandle(p_entry,current_time);
}

X_Void UartIrqCallBack_2(uint8_t data)
{
   UartIrqHandle(p_entry,data);
}


