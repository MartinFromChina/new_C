#include "b_StateMachine.h"

X_Boolean BSM_Run( const s_B_StateMachine *p_bsm,s_BSM_Param *p_param_base,uint32_t ms)
{
    StateNumber temp_state;

	if(p_bsm == (s_B_StateMachine *)0 || p_param_base == (s_BSM_Param *)0) {return X_False;}
	if(p_bsm->p_Action_buf == (BSM_Action *)0){return X_False;}    // no need pointer check ?
	if(p_bsm->TotalStateNum > MAX_B_STATE_NUMBER || p_bsm->TotalStateNum == 0) {return X_False;}

    // 1,load cur state from innear or outside suddenchange 
	temp_state = (*p_bsm->p_CurrentStateNum);
	if(p_bsm ->is_state_self_ctrl != X_True 
                && *p_bsm ->p_SuddenChangeState < p_bsm->TotalStateNum) 
	{
		temp_state = *p_bsm ->p_SuddenChangeState;
		*p_bsm ->p_SuddenChangeState = INVALID_B_STATE_NUMBER;
	}

	if(p_bsm ->p_Action_buf[temp_state] != (BSM_Action)0)
	{
        // 2,run cur_state action , get next state 
		p_param_base->cur_state = temp_state;
		temp_state = p_bsm ->p_Action_buf[temp_state](p_param_base,ms); 
		
        // 3,restore next state 

        if(temp_state == B_STATE_NUMBER_STILL)
        {
            temp_state = p_param_base->cur_state;
        }
        else
        {
            p_param_base -> cur_line_num = 0;
        }

		if(temp_state >= p_bsm->TotalStateNum)
		{
			(*p_bsm->p_CurrentStateNum) = DEFAULT_B_STATE_NUMBER;
			return X_False;
		}
		(*p_bsm->p_CurrentStateNum) = temp_state;
		
        return X_True;
	}
	return X_False;
}

X_Boolean BSM_SetState(const s_B_StateMachine *p_bsm,StateNumber state)
{
    if(p_bsm == (s_B_StateMachine *)0) {return X_False;}
    if(p_bsm ->is_state_self_ctrl == X_True)  {return X_False;}
	if((state + 1) > p_bsm->TotalStateNum ) {return X_False;}
	*p_bsm ->p_SuddenChangeState = state;
	return X_True;
}
