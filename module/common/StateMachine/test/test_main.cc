#include <iostream>
#include "tdd_common.h"

#include "../StateMachine.h"
using namespace std;
using namespace testing; // testing :: Return   ; testing :: AnyNumber   !!!

/**************************************************************************/
typedef struct
{
	s_StateMachineParam 		base;
	StateNumber 						StateBackupWhenSuccessed;
	StateNumber 						StateBackupWhenFailed;
	uint16_t 								wait_counter;
}sParamExtern;

static sParamExtern sPE,sPE1;

typedef enum
{
	RWC_Idle = 0,
	RWC_Second ,
}RWC_state;

static StateNumber RWC_IdleAction(s_StateMachineParam *p_this)
{
	UNUSED_VARIABLE(p_this);
	return RWC_Second;
}
static StateNumber RWC_SecondAction(s_StateMachineParam *p_this)
{
	UNUSED_VARIABLE(p_this);
	return RWC_Idle;
}


static const StateAction SimpleStateAction[] = {
		{RWC_IdleAction},
		{RWC_SecondAction},
};

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	UNUSED_VARIABLE(state_going_to_leave);
	UNUSED_VARIABLE(state_going_to_enter);

	 EXPECT_EQ(2,2);
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

class Mock_StateFoo {
public:
    MOCK_METHOD0(ExpectCurrentState,	uint8_t());
	MOCK_METHOD0(ExpectCurrentState1,	uint8_t());
	MOCK_METHOD0(ExpectCurrentState2,	uint8_t());
};

Mock_StateFoo mocker_sf;

static StateNumber t0Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState()); return 1;}
static StateNumber t1Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState()); return 2;}
static StateNumber t2Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState()); return 3;}
static StateNumber t3Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState()); return 4;}
static StateNumber t4Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState()); return 0;}

typedef X_Void (*f_jump_recorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);


static const StateAction jumpStateAction[] = {
		{t0Action},{t1Action},{t2Action},{t3Action},{t4Action},
};

APP_STATE_MACHINE_DEF(p_jump_state
						,sizeof(jumpStateAction)/sizeof(jumpStateAction[0])
						,&jumpStateAction[0]);


TEST(state_machine,state_jump)
{
	uint8_t i;
	EXPECT_CALL(mocker_sf, ExpectCurrentState()).Times(5).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(2))
														  .WillOnce(Return(3)).WillOnce(Return(4));
	for(i=0;i<5;i++)
	{
		mStateMachineRun(p_jump_state,&sPE.base,(f_jump_recorder)0);
	}
	
}


static StateNumber t00Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState1()); return 3;}
static StateNumber t01Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState1()); return 0;}
static StateNumber t02Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState1()); return 4;}
static StateNumber t03Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState1()); return 2;}
static StateNumber t04Action(s_StateMachineParam *p_this){EXPECT_EQ(p_this ->current_state,mocker_sf.ExpectCurrentState1()); return 1;}

typedef X_Void (*f_jump_recorder)(StateNumber state_going_to_leave,StateNumber state_going_to_enter);


static const StateAction jumpStateAction0[] = {
		{t00Action},{t01Action},{t02Action},{t03Action},{t04Action},
};

APP_STATE_MACHINE_DEF(p_mul_state
						,sizeof(jumpStateAction0)/sizeof(jumpStateAction0[0])
						,&jumpStateAction0[0]);

TEST(state_machine,mul_entry)
{
	uint8_t i;
	EXPECT_CALL(mocker_sf, ExpectCurrentState()).Times(5).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(2))
														  .WillOnce(Return(3)).WillOnce(Return(4));
	EXPECT_CALL(mocker_sf, ExpectCurrentState1()).Times(5).WillOnce(Return(0)).WillOnce(Return(3)).WillOnce(Return(2))
														  .WillOnce(Return(4)).WillOnce(Return(1));
	for(i=0;i<5;i++)
	{
		mStateMachineRun(p_jump_state,&sPE.base,(f_jump_recorder)0);
		mStateMachineRun(p_mul_state,&sPE1.base,(f_jump_recorder)0);
	}
}

TEST(state_machine,boundary)
{

}

TEST(state_machine,param)
{

}


static StateNumber t000Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 1;}
static StateNumber t001Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 2;}
static StateNumber t002Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 3;}
static StateNumber t003Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 4;}
static StateNumber t004Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 0;}

static const StateAction jumpStateAction00[] = {
		{t000Action},{t001Action},{t002Action},{t003Action},{t004Action},
};

APP_STATE_MACHINE_DEF(p_record_state
						,sizeof(jumpStateAction00)/sizeof(jumpStateAction00[0])
						,&jumpStateAction00[0]);


static X_Void JumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	 EXPECT_EQ(state_going_to_leave,mocker_sf.ExpectCurrentState2());
	 EXPECT_EQ(state_going_to_enter,mocker_sf.ExpectCurrentState2());
}

TEST(state_machine,jump_recorder)
{
	uint8_t i;
	EXPECT_CALL(mocker_sf, ExpectCurrentState2()).Times(10).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(1))
														  .WillOnce(Return(2)).WillOnce(Return(2)).WillOnce(Return(3))
														  .WillOnce(Return(3)).WillOnce(Return(4)).WillOnce(Return(4))
														  .WillOnce(Return(0));
	for(i=0;i<5;i++)
	{
		mStateMachineRun(p_record_state,&sPE.base,JumpRecorder);
	}
}








GTEST_API_ int main(int argc, char **argv)
{
	cout<<"state machine test from here \r\n"<<endl;
	 testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}
