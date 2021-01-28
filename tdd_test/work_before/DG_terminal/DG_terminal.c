#include "DG_terminal.h"
#include "DG_protocol.h"

#define RECV_DEBUG        1

static uint32_t systime_timer = 0;
static func_send p_send = (func_send)0;
static uint8_t curr_recv = 0;
static uint8_t header_index = 0, temp_index = 0;
static uint8_t terminal_backup = 0xff;
//////////////static X_Boolean isNewFrameHasCome = X_False;


X_DATA_UNIT 			DG_unit_receive(X_Void) {return curr_recv;}
X_Boolean 				DG_find_header(X_DATA_UNIT current_data)
{
	if(header_index == 0 && current_data == 0xaa) {header_index = 1;return X_False;} // there is a problem : mul entry all use one header_index !!! 

	if(header_index == 1 && current_data == 0x55)
	{
		header_index = 0;
		return X_True;
	}
	header_index = 0;
	return X_False;
}
static X_Boolean CheckSum(uint8_t *p_buf,uint8_t length)
{
	uint16_t i;
	uint8_t sum = 0;
	if(length == 0 || length == 255) {return X_False;}
	for(i=0;i<length - 1;i++)
	{
		sum += p_buf[i];
	}
	if(sum == p_buf[length - 1]) {return X_True;}
	return X_False;
}

e_find_other_process 	DG_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf)
{
	///////////uint8_t i;
	if(*p_fop == FOP_idle)
	{
		if(current_data < 5) {return FOP_idle;}
		p_buf[1] = 0xaa;
		p_buf[2] = 0x55;
		p_buf[3] = current_data;
		temp_index = 4;
		return FOP_inprocess;
	}

	if(*p_fop == FOP_inprocess)
	{
		p_buf[temp_index] = current_data;
		//printf("---------p_buf[%d]=  %2x\r\n",temp_index,p_buf[temp_index]);
		temp_index++;
		if(temp_index == (p_buf[3] + 1)) 
		{
			if(CheckSum(&p_buf[1],p_buf[3]) == X_True)// check sum OK
			{
				/*
				printf("receive successed length %d ;",p_buf[3]);
				for(i = 0;i<=p_buf[3];i++)
				{
					printf(" %2x",p_buf[i]);
				}
				printf("\r\n");
				*/
				return FOP_successed;
			}
			else
			{	
			/*
				INSERT(LogDebug)(RECV_DEBUG,("receive failed length %d ;",p_buf[3]));
				for(i = 0;i<=p_buf[3];i++)
				{
					INSERT(LogDebug)(RECV_DEBUG,(" %2x",p_buf[i]));
				}
				INSERT(LogDebug)(RECV_DEBUG,("\r\n"));
				*/
				return FOP_idle;
			}
		}
		else
		{
			return FOP_inprocess;
		}
	}
	
	return FOP_idle;
}


X_Void DG_TerminalInit(func_send p_src)
{
	p_send = p_src;
	curr_recv = 0;
	header_index = 0;
	temp_index = 0;
	///////////isNewFrameHasCome = X_False;
}

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time)
{
	X_Boolean isOK;
	uint8_t *p_buf,i;
	// lock irq
	isOK = p_terminal ->p_action ->GetFrame(p_terminal ->p_action ->p_manager,&p_buf);
	if(isOK == X_True && p_buf != X_Null)
	{
		///////////isNewFrameHasCome = X_True;
		if(terminal_backup != p_terminal ->terminal_num)
		{
			terminal_backup = p_terminal ->terminal_num;
			INSERT(LogDebug)(RECV_DEBUG,("------------------------------------------------ \r\n"));
		}
		INSERT(LogDebug)(RECV_DEBUG,("terminal %d receive data ",p_terminal ->terminal_num));
		for(i=0;i<p_buf[2];i++)
		{
			INSERT(LogDebug)(RECV_DEBUG,("%2x,",p_buf[i]));
		}
		INSERT(LogDebug)(RECV_DEBUG,(" at time %d \r\n",systime_timer));
		
	}
	// un lock irq
	systime_timer = current_time;
}

static const s_terminal * p_terminal_backup = (s_terminal *)0; // const ?
X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data)
{
	 curr_recv = data;
	 if(p_terminal != p_terminal_backup)
	 {
		header_index = 0;
		temp_index = 0;
		p_terminal_backup = p_terminal;
	 }
	 p_terminal ->p_action ->Process(p_terminal ->p_action ->p_manager);
	 /////////////////////INSERT(LogDebug)(RECV_DEBUG,("-------------------------terminal %d receive data %2x \r\n",p_terminal ->terminal_num,data));
	/*
	
	if(p_terminal ->terminal_num == 1 && data == 0)
	{
		if(p_send != X_Null) 
		{
			p_send(p_terminal ->terminal_num,systime_timer,buf,4);
		}
	}
	*/
}
