#include "DG_terminal.h"
#include "DG_protocol.h"

#define RECV_DEBUG        1

uint32_t systime_timer = 0;
static func_send p_send = (func_send)0;

X_DATA_UNIT empty_unit_receive(X_Void){return 0;}
X_Boolean empty_find_header(X_DATA_UNIT current_data){UNUSED_PARAMETER(current_data);return X_True;}
e_find_other_process empty_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf)
{UNUSED_PARAMETER(current_data);UNUSED_PARAMETER(p_fop); UNUSED_PARAMETER(p_buf);return FOP_successed;}


X_Void SentFuncInit(func_send p_src)
{
	p_send = p_src;
}

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time)
{
	UNUSED_PARAMETER(p_terminal);
	systime_timer = current_time;
}

static uint8_t terminal_backup = 0xff;
X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data)
{
	uint8_t buf[4] = {9,8,7,6};
	if(terminal_backup != p_terminal ->terminal_num)
	{
		terminal_backup = p_terminal ->terminal_num;
		INSERT(LogDebug)(RECV_DEBUG,("------------------------------------------------ \r\n"));
	}
	INSERT(LogDebug)(RECV_DEBUG,("terminal %d receive data %2x at time %d \r\n",p_terminal ->terminal_num,data,systime_timer));
	
	if(p_terminal ->terminal_num == 1 && data == 0)
	{
		if(p_send != X_Null) 
		{
			p_send(p_terminal ->terminal_num,systime_timer,buf,4);
		}
	}
}
