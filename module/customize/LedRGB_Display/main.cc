#include "../common/StateMachine/StateMachine.h"

StateNumber LS_IdleAction(s_StateMachineParam *p_this)
{

}
StateNumber LS_LoadEventAction(s_StateMachineParam *p_this)
{

}

static const StateAction SimpleStateAction[] = {						
				{LS_IdleAction},																		
				{LS_LoadEventAction},																		
		};			
				
APP_STATE_MACHINE_DEF(p_manager,																				
					sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0])		
					,&SimpleStateAction[0]);			


int main(int argc, char **argv)
{
	mStateMachineStateSet(p_manager,0);
}
