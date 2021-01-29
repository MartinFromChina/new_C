#include "DG_terminal.h"
#include "DG_protocol.h"
#include "DG_terminal_interconnect.h"


#define RECV_DEBUG        1
#define FIND_HEADER_DEBUG        0
#define CHECK_SUM_DEBUG        0



static uint32_t systime_timer = 0;
static func_send p_send = (func_send)0;
static uint8_t curr_recv = 0;
static uint8_t header_index = 0, temp_index = 0;
//static uint8_t terminal_backup = 0xff;
static uint8_t temp_rec_buf[MAX_DG_FRAME_LENGTH],temp_send_buf[MAX_DG_FRAME_LENGTH];


X_DATA_UNIT 			DG_unit_receive(X_Void) {return curr_recv;}
X_Boolean 				DG_find_header(X_DATA_UNIT current_data)
{
	if((header_index == 0 || header_index == 0x30)  && current_data == 0xaa) {header_index = 0x01;return X_False;} // there is a problem : mul entry all use one header_index !!! 
	else if((header_index == 0 || header_index == 0x30)&& current_data == 0xcc){header_index = 0x11;return X_False;}
	if(header_index == 1 && current_data == 0x55)
	{
		header_index = 0;
		return X_True;
	}
	else if (header_index == 0x11 && current_data == 0x66)
	{
		INSERT(LogDebug)(FIND_HEADER_DEBUG,("*****************************************find header 0x66cc ;\r\n"));
		header_index = 0x30;
		return X_True;
	}
	header_index = 0;
	return X_False;
}
static X_Boolean CheckSum(uint8_t *p_buf,uint8_t length)
{
	uint16_t i;
	uint8_t sum = 0;
	if(length < 5 || length == 255) {return X_False;}
	for(i=0;i<length - 1;i++)
	{
		sum += p_buf[i];
	}
	if(sum == p_buf[length - 1]) {return X_True;}
	else
	{
		INSERT(LogDebug)(FIND_HEADER_DEBUG,("xxxxxxxxxxxx sum %2x != %2x;\r\n",sum,p_buf[length - 1]));
	}
	return X_False;
}

e_find_other_process 	DG_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf)
{
	uint16_t *p_length,length;
	if(*p_fop == FOP_idle)
	{
		if(current_data < 5) {return FOP_idle;}

		if(header_index == 0x30)
		{
			p_buf[1] = 0xcc;
			p_buf[2] = 0x66;
			p_buf[3] = current_data; // fix it later
			temp_index = 4;//!!!! very important
			INSERT(LogDebug)(FIND_HEADER_DEBUG,("*****************************************find others %d length %d ;\r\n",current_data,GetLength(&p_buf[1])));
		}
		else
		{
			p_buf[1] = 0xaa;
			p_buf[2] = 0x55;
			p_buf[3] = current_data;
			temp_index = 4;
		}
		
		return FOP_inprocess;
	}

	if(*p_fop == FOP_inprocess)
	{
		p_buf[temp_index] = current_data;
		//////////////if(header_index == 0x30) printf("---------p_buf[%d]=  %2x\r\n",temp_index,p_buf[temp_index]);
		temp_index++;
		/////////////if(header_index == 0x30){INSERT(LogDebug)(FIND_HEADER_DEBUG,("*****find complete index %d;length %d ;\r\n",temp_index,GetLength(&p_buf[1])));}
		length = GetLength(&p_buf[1]);
		if(temp_index >= (length + 1)) 
		{
			if(CheckSum(&p_buf[1],GetLength(&p_buf[1])) == X_True)// check sum OK
			{
				/*
				printf("receive successed length %d ;",p_buf[3]);
				for(i = 0;i<=p_buf[3];i++)
				{
					printf(" %2x",p_buf[i]);
				}
				printf("\r\n");
				*/
				/////////////if(header_index == 0x30){INSERT(LogDebug)(FIND_HEADER_DEBUG,("************************find complete %d ;\r\n",GetLength(&p_buf[1])));}
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
				//////////////if(header_index == 0x30){INSERT(LogDebug)(FIND_HEADER_DEBUG,("************************find complete check sum failed; %d ;\r\n",GetLength(&p_buf[1])));}
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
	DG_InterconnectInit();
}

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time)
{
	X_Boolean isOK,isNewFrameCome = X_False,isStillWaitAck;
	uint8_t *p_buf,i,data_or_command_type,src;
	s_DG_immedicate_ack ime_ack,*p;
	p = &ime_ack;
	// lock irq
	isOK = p_terminal ->p_action ->GetFrame(p_terminal ->p_action ->p_manager,&p_buf);
	if(isOK == X_True && p_buf != X_Null && p_buf[2] < MAX_DG_FRAME_LENGTH)
	{
		isNewFrameCome = X_True;
		for(i=0;i<p_buf[2];i++)
		{
			temp_rec_buf[i] = p_buf[i];
		}
	}
	// unlock irq
	isStillWaitAck = ImmediatelyAckWaiting(p_terminal,p_send,isNewFrameCome,temp_rec_buf,current_time); 
	if(isNewFrameCome == X_True)
	{
		data_or_command_type = GetType(temp_rec_buf);
		src                  = GetSrc(temp_rec_buf);
		if(p_send != X_Null && (data_or_command_type != IMMEDIATELY_ACK_TYPE) 
				&&(DoesItAboutMe(p_terminal -> terminal_num,src,GetDest(temp_rec_buf) ) == X_True ))  // immediately ack
		{
			ime_ack.header = 0x66cc;
			ime_ack.length = sizeof(ime_ack)/sizeof(uint8_t);
			ime_ack.src    = p_terminal ->terminal_num;
			ime_ack.dest   = src;
			ime_ack.type   = IMMEDIATELY_ACK_TYPE;
			ime_ack.data_or_command_type = data_or_command_type;
			p_send(p_terminal ->terminal_num,systime_timer,(uint8_t *)p,sizeof(ime_ack)/sizeof(uint8_t));
		}
		
		
		if(isStillWaitAck == X_False) // means ack finished or no need to wait ack
		{
			isOK = TerminalInterconnectHandle(p_terminal,temp_rec_buf,temp_send_buf,current_time);
			if(isOK == X_True)
			{
				if(p_send != X_Null) {p_send(p_terminal ->terminal_num,systime_timer,temp_send_buf,temp_send_buf[2]);}
			}
		}
		
	}
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
}

