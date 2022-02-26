#include "StateMachine.h"

m_app_result mStateMachineRun( const s_StateMachine *p_ssp
						,s_StateMachineParam *p_smp
						,X_Void(*StateRecorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter))
{
	StateNumber current_state,previous_state;

	if(p_ssp == X_Null || p_smp == X_Null) {return APP_POINTER_NULL;}
	if(p_ssp->p_Action == X_Null){return APP_POINTER_NULL;}
	if(p_ssp->AllStateNum > MAX_STATE_NUMBER || p_ssp->AllStateNum == 0) {return APP_BEYOND_SCOPE;}

	current_state = (*p_ssp->p_CurrentStateNum);
	if(*p_ssp ->p_SuddenChangeState != INVALID_STATE_NUMBER) 
	{
		current_state = *p_ssp ->p_SuddenChangeState;
		*p_ssp ->p_SuddenChangeState = INVALID_STATE_NUMBER;
	}
	if(current_state > MAX_STATE_NUMBER || (current_state + 1) > p_ssp->AllStateNum)
	{
		(*p_ssp->p_CurrentStateNum) = DEFAULT_STATE_NUMBER;
		return APP_BEYOND_SCOPE;
	}

	if(p_ssp ->p_Action[current_state].Action != X_Null)
	{
		previous_state = current_state;
		p_smp->current_state = current_state;
		current_state = p_ssp ->p_Action[current_state].Action(p_smp);


		if(*p_ssp ->p_SuddenChangeState != INVALID_STATE_NUMBER) 
		{
			current_state = *p_ssp ->p_SuddenChangeState;
			*p_ssp ->p_SuddenChangeState = INVALID_STATE_NUMBER;
		}
		
		if((current_state+1) > p_ssp->AllStateNum)
		{
			(*p_ssp->p_CurrentStateNum) = DEFAULT_STATE_NUMBER;
			return APP_BEYOND_SCOPE;
		}
		(*p_ssp->p_CurrentStateNum) = current_state;
		
		if(previous_state != current_state)
		{
			if(StateRecorder != X_Null) {StateRecorder(previous_state,current_state);}
		}
	}
	return APP_SUCCESSED;
}

m_app_result mStateMachineStateSet(const s_StateMachine *p_ss,StateNumber state)
{
	if(p_ss == X_Null) {return APP_POINTER_NULL;}
	if((state + 1) > p_ss->AllStateNum ) {return APP_BEYOND_SCOPE;}
	*p_ss ->p_SuddenChangeState = state;
	return APP_SUCCESSED;
}

StateNumber mStateMachineGetSuddenChangeState(const s_StateMachine *p_ss)
{
	if(p_ss == X_Null) {return APP_POINTER_NULL;}
	return *p_ss ->p_SuddenChangeState;
}
