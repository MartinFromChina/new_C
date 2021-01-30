#include "DG_terminal_interconnect.h"
#include "DG_protocol.h"
#include "../../../module/common/StateMachine/StateMachine.h"

#define STATE_MACHINE_DEBUG  0
#define IMME_ACK_DEBUG       0
#define TRANS_DEBUG          0
#define ERROR_REPORT_DEBUG   0

INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);
typedef struct
{
	s_StateMachineParam 		base;
	X_Boolean 					isStateRun;
	X_Boolean                   isSendData;
	const s_terminal 			*p_terminal;
	uint8_t 					*p_recv;
	uint8_t 					*p_send;
	StateNumber                 state_remain;
	uint32_t                    sys_time;
}sParamExtern;
static sParamExtern sPE;

typedef enum
{
	S_Idle = 0,
	S_CommandAnalysis,
	S_End,
}s_state;

static X_Void WaitAckParamLoad(s_wait_ack *p_wait,uint8_t wait_src,uint8_t wait_type,uint8_t src,uint32_t time,uint8_t *p_buf)
{
	p_wait->isExpectAckHasCome     = X_False;
	p_wait->isReSend            = X_False;
	p_wait->wait_counter 		= MAX_ADJACENT_TERMINAL_DISTANCE * 2;
	p_wait->wait_source  		= wait_src;
	p_wait->command_start_terminal = src;
	p_wait->start_wait_time 	= time;
	p_wait->wait_type          = wait_type;
	p_wait->wait_times         = 1;
	CopyFrame(p_buf,p_wait->resend_buf);
}
	
static StateNumber S_IdleAction(s_StateMachineParam *p_this);
static StateNumber S_CommandAnalysisAction(s_StateMachineParam *p_this);
static StateNumber S_EndAction(s_StateMachineParam *p_this);

static const StateAction TerminalStateAction[] = {
		{S_IdleAction},
		{S_CommandAnalysisAction},
		{S_EndAction},
};

APP_STATE_MACHINE_DEF(p_state
								,sizeof(TerminalStateAction)/sizeof(TerminalStateAction[0])
								,&TerminalStateAction[0]);

typedef X_Void (*state_record)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);
static StateNumber S_IdleAction(s_StateMachineParam *p_this){

	sParamExtern * p_ext= (sParamExtern*)p_this;
	p_ext ->isSendData = X_False;
	return S_CommandAnalysis;
}
static StateNumber S_CommandAnalysisAction(s_StateMachineParam *p_this){
	e_frame_type frame_type;
	uint8_t src,dest,me,type;
   sParamExtern * p_ext= (sParamExtern*)p_this;
   s_wait_ack   *p_wait = p_ext ->p_terminal ->p_wait_ack;
   src 	= GetSrc(p_ext ->p_recv);
   dest = GetDest(p_ext ->p_recv);
   me   = p_ext ->p_terminal ->terminal_num;
   type = GetType(p_ext ->p_recv);
   CopyFrame(p_ext ->p_recv,p_ext ->p_send);

   if(p_ext ->p_recv[0] == 0xaa || p_ext ->p_recv[0] == 0x55) {frame_type = eft_command;}
   else {frame_type = eft_data;}
   if(me == dest)  // for me
   {	INSERT(LogDebug)(ERROR_REPORT_DEBUG & (p_ext ->p_terminal ->p_wait_ack ->isStartPoint == X_True) ,(" : ************************************meet touch point*********************************************;\r\n"));
   		if(src == p_ext ->p_terminal ->backward_num )
   		{
			INSERT(LogDebug)(TRANS_DEBUG | ERROR_REPORT_DEBUG,(" : for me from backward!!!!!!;\r\n"));
			if(type == ERROR_REPORT_TYPE && p_ext ->p_terminal ->p_wait_ack ->isStartPoint == X_False)
   			{
				p_ext ->isSendData = X_True;
				SetSrcDest(p_ext ->p_send,me,p_ext ->p_terminal ->forward_num);
				return S_End;
			}
			p_ext ->isSendData = DG_CommandHandle(p_ext ->p_terminal,frame_type,efd_for_me,type,p_ext ->p_recv,p_ext ->p_send);
   		}
		else if(src == p_ext ->p_terminal ->forward_num)
		{
			INSERT(LogDebug)(TRANS_DEBUG | ERROR_REPORT_DEBUG ,(" : for me from forward!!!!!!;\r\n"));
			if(type == ERROR_REPORT_TYPE && p_ext ->p_terminal ->p_wait_ack ->isStartPoint == X_False)
   			{
				p_ext ->isSendData = X_True;
				SetSrcDest(p_ext ->p_send,me,p_ext ->p_terminal ->backward_num);
				return S_End;
			}
			p_ext ->isSendData = DG_CommandHandle(p_ext ->p_terminal,frame_type,efd_for_me,type,p_ext ->p_recv,p_ext ->p_send);
		}
		else {INSERT(LogDebug)(TRANS_DEBUG,(" :for me but not from neighbour terminal ,ignore it !!!!!!;\r\n"));}

		return S_End;
   }
   else if(me < dest && me > src) // trans down
   {
   		if(src != p_ext ->p_terminal ->forward_num) {INSERT(LogDebug)(TRANS_DEBUG,(" : ignore it !!!!!!\r\n"));return S_End;} // only accept adjacent terminal trans down
		if(DoesMultiCastType(type) == X_False)
		{
			p_ext ->isSendData = X_True;
			SetSrcDest(p_ext ->p_send,me,dest);

			WaitAckParamLoad(p_wait,p_ext ->p_terminal ->backward_num,type,src,p_ext ->sys_time,p_ext ->p_send);
			INSERT(LogDebug)(TRANS_DEBUG,("!!!!!!terminal %d trans down at time %d :\r\n",p_ext ->p_terminal ->terminal_num,p_ext ->sys_time));
		}
		else
		{
			p_ext ->isSendData = DG_CommandHandle(p_ext ->p_terminal,frame_type,efd_trans_down,type,p_ext ->p_recv,p_ext ->p_send);
			if(p_ext ->isSendData == X_True) {WaitAckParamLoad(p_wait,p_ext ->p_terminal ->backward_num,type,src,p_ext ->sys_time,p_ext ->p_send);}
		}
		///////////////////INSERT(LogDebug)(IMME_ACK_DEBUG,("terminal %d wait ack begin ;time %d \r\n",p_ext ->p_terminal ->terminal_num,p_ext ->sys_time));
		return S_End;
   }
   else if(me < src && me > dest) // trans up
   {
   		if(src != p_ext ->p_terminal ->backward_num) {INSERT(LogDebug)(TRANS_DEBUG,(" : ignore it !!!!!!\r\n"));return S_End;} // only accept adjacent terminal trans down
		if(DoesMultiCastType(type) == X_False)
		{
			p_ext ->isSendData = X_True;
	   		SetSrcDest(p_ext ->p_send,me,dest);
			WaitAckParamLoad(p_wait,p_ext ->p_terminal ->forward_num,type,src,p_ext ->sys_time,p_ext ->p_send);
			INSERT(LogDebug)(TRANS_DEBUG,("!!!!!!terminal %d trans up at time %d :\r\n",p_ext ->p_terminal ->terminal_num,p_ext ->sys_time));
		}
		else
		{
			p_ext ->isSendData = DG_CommandHandle(p_ext ->p_terminal,frame_type,efd_trans_up,type,p_ext ->p_recv,p_ext ->p_send);
			if(p_ext ->isSendData == X_True) {WaitAckParamLoad(p_wait,p_ext ->p_terminal ->forward_num,type,src,p_ext ->sys_time,p_ext ->p_send);}
		}


		
		return S_End;
   }
   else // not for me 
   {
   		INSERT(LogDebug)(TRANS_DEBUG,(":not for me !!!!\r\n"));
		return S_End;
   }
   
}
static StateNumber S_EndAction(s_StateMachineParam *p_this){
	 sParamExtern * p_ext= (sParamExtern*)p_this;
	 p_ext ->isStateRun = X_False;
	 return S_Idle;
}

X_Void DG_InterconnectInit(X_Void){sPE.state_remain = MAX_STATE_NUMBER;}
X_Boolean TerminalInterconnectHandle(const s_terminal * p_terminal,uint8_t *p_recv,uint8_t *p_send,uint32_t time)
{
	sPE.isStateRun = X_True;
	sPE.p_terminal = p_terminal;
	sPE.p_recv     = p_recv;
	sPE.p_send     = p_send;
	sPE.sys_time   = time;

	while(sPE.isStateRun == X_True)
	{
		INSERT(LogDebugOnce)(STATE_MACHINE_DEBUG,p_once,sPE.base.current_state,("new state %d \r\n",sPE.base.current_state));
		mStateMachineRun(p_state,&sPE.base,(state_record)0);
	}
	return sPE.isSendData;
}

X_Boolean ImmediatelyAckWaiting(const s_terminal * p_terminal,func_send p_send_func,X_Boolean isNewFrame,uint8_t *p_recv,uint32_t time)
{
	s_wait_ack   *p_wait = p_terminal ->p_wait_ack;
	if(p_wait->wait_times == 0 && p_wait->isReSend == X_True && p_wait->isExpectAckHasCome == X_True) {return X_False;}
	if((time - p_wait->start_wait_time) > p_wait->wait_counter) // time out
	{
		if(p_wait->isReSend == X_False)// first timeout ,resend 
		{
			INSERT(LogDebug)(IMME_ACK_DEBUG | ERROR_REPORT_DEBUG ,("terminal %d first timeout ,resend ;time %d\r\n",p_terminal ->terminal_num,time));
			if(p_send_func != X_Null) {p_send_func(p_terminal ->terminal_num,time,p_wait->resend_buf,GetLength(p_wait->resend_buf));}
			p_wait->isReSend = X_True;
			p_wait->start_wait_time = time;
			return X_False;
		}
		else // second time timeout ; send error report
		{
			p_wait->wait_times = 0;p_wait->isExpectAckHasCome = X_True;
			s_ERR_report s_err;
			s_ERR_report *p_error = (s_ERR_report *)p_wait->resend_buf;
			p_error ->header = 0x66cc;
			p_error ->length = sizeof(s_err)/sizeof(uint8_t);//9;
			p_error ->src	 = p_terminal ->terminal_num;
			p_error ->dest   = p_wait->command_start_terminal;
			p_error ->type   = ERROR_REPORT_TYPE;
			p_error ->lost_terminal = p_wait->wait_source;
			INSERT(LogDebug)(IMME_ACK_DEBUG | ERROR_REPORT_DEBUG,("terminal %d second time timeout ; send error report ;time %d \r\n",p_terminal ->terminal_num,time));
			if(p_send_func != X_Null) {p_send_func(p_terminal ->terminal_num,time,p_wait->resend_buf,sizeof(s_err)/sizeof(uint8_t));}
			return X_True;
		}
	}
	else
	{
		if(isNewFrame == X_True)
		{
			if(DoesImediatelyAckForMe(p_recv,p_terminal ->terminal_num,p_wait->wait_source,p_wait->wait_type) == X_True)
			{
				INSERT(LogDebug)(IMME_ACK_DEBUG,("terminal %d get imme ack !!! ;time %d \r\n",p_terminal ->terminal_num,time));
				p_wait->wait_times = 0;p_wait->isExpectAckHasCome = X_True;p_wait->isReSend = X_True;
				return X_False;
			}
		}
	}
	return X_False;
}

