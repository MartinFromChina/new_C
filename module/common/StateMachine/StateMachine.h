#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../x_cross_platform.h"
#include "../AppError.h"
#include "../CommonMarco.h"

#define INVALID_STATE_NUMBER		(0xFF)
#define MAX_STATE_NUMBER    		(0xFE)
#define DEFAULT_STATE_NUMBER        0



typedef uint8_t 			StateNumber;

/************************************* state machine*****************************************************************/
typedef  struct
{
	StateNumber current_state;
}s_StateMachineParam;

typedef  struct {
	StateNumber (*Action)(s_StateMachineParam *p_this);
}StateAction;
typedef struct
{
	const StateNumber 	AllStateNum;
	StateAction const 	*p_Action;
    StateNumber         *p_CurrentStateNum;
	StateNumber         *p_SuddenChangeState;
}s_StateMachine;

#define APP_STATE_MACHINE_DEF_WITHOUT_POINTER(id,state_number,p_action)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static StateNumber CONCAT_2(id, _sudden_change_state_number) = INVALID_STATE_NUMBER;						\
static const s_StateMachine CONCAT_2(id, _entry) = {state_number,p_action,&CONCAT_2(id, _current_state_number),&CONCAT_2(id, _sudden_change_state_number)}


#define APP_STATE_MACHINE_DEF(id,state_number,p_action)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static StateNumber CONCAT_2(id, _sudden_change_state_number) = INVALID_STATE_NUMBER;						\
static const s_StateMachine CONCAT_2(id, _entry) = {state_number,p_action,&CONCAT_2(id, _current_state_number),&CONCAT_2(id, _sudden_change_state_number)}; \
static const s_StateMachine* id = &CONCAT_2(id, _entry)

m_app_result mStateMachineRun( const s_StateMachine *p_ssp
						,s_StateMachineParam *p_smp
						,X_Void(*StateRecorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter));

m_app_result mStateMachineStateSet(const s_StateMachine *p_ss,StateNumber state);


#ifdef __cplusplus
		}
#endif

#endif


/*
******************************************************
example code
******************************************************
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
	RWC_Idle = 0,
}RWC_state;

**********************************************************************************************************************************
			0
*********************************************************************************************************************************
static StateNumber RWC_IdleAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}

static const StateAction SimpleStateAction[] = {
		{RWC_IdleAction},
};

APP_STATE_MACHINE_DEF(p_simple_state
														,sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0])
														,&SimpleStateAction[0]);

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{

}

X_Void NoName(X_Void)
{
	SimpleStateMachineRun(p_simple_state,&sPE.base,StateJumpRecorder);
}
 */


