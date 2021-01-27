#include "DG_terminal.h"

uint32_t systime_timer = 0;
static func_send p_send = (func_send)0;

X_Void SentFuncInit(func_send p_src)
{
	p_send = p_src;
}

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time)
{
	systime_timer = current_time;
}

X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data)
{
	uint8_t buf[4] = {9,8,7,6};
	INSERT(LogDebug)(1,("terminal %d receive data %2x at time %d \r\n",p_terminal ->terminal_num,data,systime_timer));
	if(p_terminal ->terminal_num == 1 && data == 0)
	{
		if(p_send != X_Null) 
		{
			p_send(p_terminal ->terminal_num,systime_timer,buf,4);
		}
	}
}
