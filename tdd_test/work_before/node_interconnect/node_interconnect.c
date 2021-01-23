#include "node_interconnect.h"

#include "../../../module/common/StateMachine/StateMachine.h"


typedef struct
{
	s_StateMachineParam 		base;
	StateNumber 						StateBackupWhenSuccessed;
	StateNumber 						StateBackupWhenFailed;
	uint16_t 								wait_counter;
}sParamExtern;

static sParamExtern sPE;

typedef enum
{
	CS_Idle,
	CS_transmation,
	CS_node_receive,
	CS_node_send,
	CS_end,
}communication_state;

/**********************************************************************************************************************************
			0
*********************************************************************************************************************************/
static StateNumber CS_IdleAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CS_transmationAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CS_node_receiveAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CS_node_sendAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CS_endAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}


static const StateAction NodeStateAction[] = {
		{CS_IdleAction},
		{CS_transmationAction},
		{CS_node_receiveAction},
		{CS_node_sendAction},
		{CS_endAction},
};

APP_STATE_MACHINE_DEF(p_state
								,sizeof(NodeStateAction)/sizeof(NodeStateAction[0])
								,&NodeStateAction[0]);

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	UNUSED_PARAMETER(state_going_to_leave);
	UNUSED_PARAMETER(state_going_to_enter);
}

X_Void RunNodeCommunicationProcess(X_Void)
{
	mStateMachineRun(p_state,&sPE.base,StateJumpRecorder);
}

