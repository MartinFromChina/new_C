#include "DG_terminal.h"
uint32_t systime_timer = 0;

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time)
{
	systime_timer = current_time;
}

X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data)
{
	INSERT(LogDebug)(1,("terminal %d receive data %2x at time %d \r\n",p_terminal ->terminal_num,data,systime_timer));
}
