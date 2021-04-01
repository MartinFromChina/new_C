#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"

#include <iostream>
using namespace std;
#include "../LedDisplay.h"


//#include "../../../common/StateMachine/StateMachine.h"

StateNumber LS_IdleAction(s_StateMachineParam *p_this)
{
	return p_this ->current_state;
}
StateNumber LS_LoadEventAction(s_StateMachineParam *p_this)
{
	return p_this ->current_state;
}

static const StateAction SimpleStateAction[] = {						
				{LS_IdleAction},																		
				{LS_LoadEventAction},																		
		};			
				
APP_STATE_MACHINE_DEF(p_manager,																				
					sizeof(SimpleStateAction)/sizeof(SimpleStateAction[0])		
					,&SimpleStateAction[0]);			

APP_LOOPQUEUE_DEF(p_queue,20);

int main(int argc, char **argv)
{
	cout<<"------------led_display_test from test_main.cc \r\n";
	testing::InitGoogleTest(&argc, argv);
	
	LoopQueueInitialize(p_queue);
	mStateMachineStateSet(p_manager,0);
	
	return RUN_ALL_TESTS();
}
