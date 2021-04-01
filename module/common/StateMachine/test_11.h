#ifndef __TEST_11_H
#define __TEST_11_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../x_cross_platform.h"
#include "../AppError.h"
#include "../CommonMarco.h"

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
}s_StateMachine;

#define APP_STATE_MACHINE_DEF_WITHOUT_POINTER(id,state_number,p_action)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const s_StateMachine CONCAT_2(id, _entry) = {state_number,p_action,&CONCAT_2(id, _current_state_number)}


#define APP_STATE_MACHINE_DEF(id,state_number,p_action)                \
static StateNumber CONCAT_2(id, _current_state_number) = DEFAULT_STATE_NUMBER;						\
static const s_StateMachine CONCAT_2(id, _entry) = {state_number,p_action,&CONCAT_2(id, _current_state_number)}; \
static const s_StateMachine* id = &CONCAT_2(id, _entry)

m_app_result mStateMachineRun( const s_StateMachine *p_ssp
						,s_StateMachineParam *p_smp
						,X_Void(*StateRecorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter));

m_app_result mStateMachineStateSet(const s_StateMachine *p_ss,StateNumber state);

#ifdef __cplusplus
		}
#endif


#endif


