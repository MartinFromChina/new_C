#include "terminal2.h"
#include "DG_common.h"

INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);


X_Void main_loop_2(X_Void)
{
   INSERT(LogDebugCounter)(1,p_counter,30,("main2 still alive \r\n"));
}
