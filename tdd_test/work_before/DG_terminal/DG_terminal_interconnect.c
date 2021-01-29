#include "DG_terminal_interconnect.h"
#include "DG_protocol.h"
#include "../../../module/common/StateMachine/StateMachine.h"

#define STATE_MACHINE_DEBUG  0
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);

typedef struct
{
	s_StateMachineParam 		base;
	X_Boolean 					isStateRun;
	X_Boolean                   isSendData;
	const s_terminal 			*p_terminal;
	uint8_t 					*p_recv;
	uint8_t 					*p_send;
}sParamExtern;

static sParamExtern sPE;

typedef enum
{
	S_Idle = 0,
	S_WaitAck,
	S_CommandAnalysis,
	//S_TansDown,
	//S_TransUp,
	//S_ForMe,
	S_End,
}s_state;
	
static StateNumber S_IdleAction(s_StateMachineParam *p_this);
static StateNumber S_WaitAckAction(s_StateMachineParam *p_this);
static StateNumber S_CommandAnalysisAction(s_StateMachineParam *p_this);
static StateNumber S_EndAction(s_StateMachineParam *p_this);

static const StateAction TerminalStateAction[] = {
		{S_IdleAction},
		{S_WaitAckAction},
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
static StateNumber S_WaitAckAction(s_StateMachineParam *p_this){
	 sParamExtern * p_ext= (sParamExtern*)p_this;
	 return p_this ->current_state;
}
static StateNumber S_CommandAnalysisAction(s_StateMachineParam *p_this){

	uint8_t src,dest,me;
   //	if not for me ,return idle and stop state
   sParamExtern * p_ext= (sParamExtern*)p_this;
   src 	= GetSrc(p_ext ->p_recv);
   dest = GetDest(p_ext ->p_recv);
   me   = p_ext ->p_terminal ->terminal_num;
   CopyFrame(p_ext ->p_recv,p_ext ->p_send);
   if(me == dest)  // for me
   {
		return S_End;
   }
   else if(me < dest && me > src) // trans down
   {
		p_ext ->isSendData = X_True;
		SetSrcDest(p_ext ->p_send,me,dest);
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

X_Boolean TerminalInterconnectHandle(const s_terminal * p_terminal,uint8_t *p_recv,uint8_t *p_send)
{
	sPE.isStateRun = X_True;
	sPE.p_terminal = p_terminal;
	sPE.p_recv     = p_recv;
	sPE.p_send     = p_send;

	while(sPE.isStateRun == X_True)
	{
		INSERT(LogDebugOnce)(STATE_MACHINE_DEBUG,p_once,sPE.base.current_state,("new state %d \r\n",sPE.base.current_state));
		mStateMachineRun(p_state,&sPE.base,(state_record)0);
	}
	return sPE.isSendData;
}


