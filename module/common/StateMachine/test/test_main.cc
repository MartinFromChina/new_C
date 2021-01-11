#include <iostream>
#include "tdd_common.h"

#include "../StateMachine.h"
using namespace std;

/**************************************************************************/
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

static StateNumber RWC_IdleAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}

static const StateAction SimpleStateAction[] = {
		{RWC_IdleAction},
};

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{

}

/**************************************************************************/



TEST(state_machine,init)
{
	uint16_t i;

	static const s_StateMachine* p_state = (s_StateMachine*)0;

	EXPECT_EQ(APP_POINTER_NULL, mStateMachineRun(p_state,&sPE.base,StateJumpRecorder));
	
	APP_STATE_MACHINE_DEF(p_simple_state
						,sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0])
						,&SimpleStateAction[0]);

	for(i=0;i<100;i++)
	{
	// APP_ERROR
		EXPECT_EQ(APP_SUCCESSED, mStateMachineRun(p_simple_state,&sPE.base,StateJumpRecorder));
		EXPECT_EQ(APP_POINTER_NULL, mStateMachineRun(p_simple_state,(s_StateMachineParam*)0,StateJumpRecorder));
	}
	
						
}




GTEST_API_ int main(int argc, char **argv)
{
	cout<<"state machine test from here \r\n"<<endl;
	 testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}
