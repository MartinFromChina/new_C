#include "DG_terminal_interconnect.h"
#include "DG_protocol.h"
#include "../../../module/common/StateMachine/StateMachine.h"

#define STATE_MACHINE_DEBUG  0
#define IMME_ACK_DEBUG       1
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

static s_wait_ack   sWA;
static sParamExtern sPE;

typedef enum
{
	S_Idle = 0,
	S_CommandAnalysis,
	//S_TansDown,
	//S_TransUp,
	//S_ForMe,
	S_End,
}s_state;
	
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
	
	/*if(p_ext ->state_remain <= S_End)
	{
		p_ext ->state_remain = MAX_STATE_NUMBER;
		return p_ext ->state_remain;
	}*/
	return S_CommandAnalysis;
}
static StateNumber S_CommandAnalysisAction(s_StateMachineParam *p_this){

	uint8_t src,dest,me,type;
   //	if not for me ,return idle and stop state
   sParamExtern * p_ext= (sParamExtern*)p_this;
   src 	= GetSrc(p_ext ->p_recv);
   dest = GetDest(p_ext ->p_recv);
   me   = p_ext ->p_terminal ->terminal_num;
   type = GetType(p_ext ->p_recv);
   CopyFrame(p_ext ->p_recv,p_ext ->p_send);
   if(me == dest)  // for me
   {
		return S_End;
   }
   else if(me < dest && me > src) // trans down
   {
		p_ext ->isSendData = X_True;
		SetSrcDest(p_ext ->p_send,me,dest);
		sWA.isExpectAckCome     = X_False;
		sWA.isReSend            = X_False;
		sWA.wait_counter 		= MAX_ADJACENT_TERMINAL_DISTANCE;
		sWA.wait_source  		= p_ext ->p_terminal ->backward_num;
		sWA.command_start_terminal = src;
		sWA.start_wait_time 	= p_ext ->sys_time;
		sWA.wait_type          = type;
		sWA.wait_times         = 1;
		CopyFrame(p_ext ->p_send,sWA.resend_buf);
		return S_End;
   }
   else if(me < src && me > dest) // trans up
   {
		p_ext ->isSendData = X_True;
		return S_End;
   }
   else // not for me 
   {
		return S_End;
   }
   
}
static StateNumber S_EndAction(s_StateMachineParam *p_this){
	 sParamExtern * p_ext= (sParamExtern*)p_this;
	 p_ext ->isStateRun = X_False;
	 return S_Idle;
}

X_Void DG_InterconnectInit(X_Void)
{
	sWA.isExpectAckCome = X_True;
	sWA.isReSend = X_True;
	sWA.wait_times = 0;
	sPE.state_remain = MAX_STATE_NUMBER;// used it as invalid flag
}
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
	if(sWA.wait_times == 0 && sWA.isReSend == X_True && sWA.isExpectAckCome == X_True) {return X_False;}
	if((time - sWA.start_wait_time) > sWA.wait_counter) // time out
	{
		if(sWA.isReSend == X_False)// first timeout ,resend 
		{
			INSERT(LogDebug)(IMME_ACK_DEBUG,("terminal %d first timeout ,resend ;time %d\r\n",p_terminal ->terminal_num,time));
			if(p_send_func != X_Null) {p_send_func(p_terminal ->terminal_num,time,sWA.resend_buf,GetLength(sWA.resend_buf));}
			sWA.isReSend = X_True;
			sWA.start_wait_time = time;
			return X_False;
		}
		else // second time timeout ; send error report
		{
			sWA.wait_times = 0;sWA.isExpectAckCome = X_True;
			s_ERR_report s_err;
			s_ERR_report *p_error = (s_ERR_report *)sWA.resend_buf;
			p_error ->header = 0x66cc;
			p_error ->length = sizeof(s_err)/sizeof(uint8_t);//9;
			p_error ->src	 = p_terminal ->terminal_num;
			p_error ->dest   = sWA.command_start_terminal;
			p_error ->type   = ERROR_REPORT_TYPE;
			p_error ->lost_terminal = sWA.wait_source;
			INSERT(LogDebug)(IMME_ACK_DEBUG,("terminal %d second time timeout ; send error report ;time %d \r\n",p_terminal ->terminal_num,time));
			if(p_send_func != X_Null) {p_send_func(p_terminal ->terminal_num,time,sWA.resend_buf,sizeof(s_err)/sizeof(uint8_t));}
			return X_True;
		}
	}
	else
	{
		if(isNewFrame == X_True)
		{
			if(DoesImediatelyAckForMe(p_recv,p_terminal ->terminal_num,sWA.wait_source,sWA.wait_type) == X_True)
			{
				INSERT(LogDebug)(IMME_ACK_DEBUG,("terminal %d get imme ack !!! ;time %d \r\n",p_terminal ->terminal_num,time));
				sWA.wait_times = 0;sWA.isExpectAckCome = X_True;sWA.isReSend = X_True;
				return X_False;
			}
		}
	}
	return X_False;
}

