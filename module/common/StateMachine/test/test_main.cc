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
	MOCK_METHOD0(ExpectCurrentState3,	uint8_t());
	MOCK_METHOD0(ExpectCurrentState4,	uint8_t());
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
static StateNumber t0000Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 1;}
static StateNumber t0001Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 2;}
static StateNumber t0002Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 3;}
static StateNumber t0003Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 4;}
static StateNumber t0004Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 5;}
static StateNumber t0005Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 6;}
static StateNumber t0006Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 7;}
static StateNumber t0007Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 8;}
static StateNumber t0008Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 9;}
static StateNumber t0009Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); return 255;}

static const StateAction jumpStateAction0000000[] = {
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0000Action},{t0001Action},{t0002Action},{t0003Action},{t0004Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},

		{t0005Action},{t0006Action},{t0007Action},{t0008Action},{t0009Action},
		
};

APP_STATE_MACHINE_DEF(p_boundary_state
						,sizeof(jumpStateAction0000000)/sizeof(jumpStateAction0000000[0])
						,&jumpStateAction0000000[0]);

static X_Void JumpRecorder000(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	UNUSED_VARIABLE(state_going_to_leave);
	UNUSED_VARIABLE(state_going_to_enter);

	 EXPECT_EQ(1,2); // should never be called
}

static X_Void JumpRecorder001(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	 EXPECT_EQ(state_going_to_leave,mocker_sf.ExpectCurrentState3());
	 EXPECT_EQ(state_going_to_enter,mocker_sf.ExpectCurrentState3());
}


//#include <stdio.h>
TEST(state_machine,boundary)
{
	//printf("jumpStateAction0000000 size %d \r\n",p_boundary_state ->AllStateNum);
	uint16_t i;
	EXPECT_CALL(mocker_sf, ExpectCurrentState3()).Times(20).WillOnce(Return(0)).WillOnce(Return(1)).WillOnce(Return(1))
														  .WillOnce(Return(2)).WillOnce(Return(2)).WillOnce(Return(3))
														  .WillOnce(Return(3)).WillOnce(Return(4)).WillOnce(Return(4))
														  .WillOnce(Return(5))
														  .WillOnce(Return(5)).WillOnce(Return(6)).WillOnce(Return(6))
														  .WillOnce(Return(7)).WillOnce(Return(7)).WillOnce(Return(8))
														  .WillOnce(Return(8)).WillOnce(Return(9)).WillOnce(Return(9))
														  .WillOnce(Return(0));
	sPE.base.current_state = 0;
	for(i=0;i<10;i++)
	{
		mStateMachineRun(p_boundary_state,&sPE.base,JumpRecorder000);
	}

	APP_STATE_MACHINE_DEF(p_boundary_state1
						,(sizeof(jumpStateAction0000000)/sizeof(jumpStateAction0000000[0]) - 1)
						,&jumpStateAction0000000[0]);
	sPE.base.current_state = 0;
	
	for(i=0;i<10;i++)
	{
		mStateMachineRun(p_boundary_state1,&sPE.base,JumpRecorder001);
	}
	
}
/*
TEST(state_machine,param)
{

}
*/

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

static StateNumber t00000Action(s_StateMachineParam *p_this);
static StateNumber t00001Action(s_StateMachineParam *p_this);
static StateNumber t00002Action(s_StateMachineParam *p_this);
static StateNumber t00003Action(s_StateMachineParam *p_this);
static StateNumber t00004Action(s_StateMachineParam *p_this);

static const StateAction jumpStateAction0000[] = {
		{t00000Action},{t00001Action},{t00002Action},{t00003Action},{t00004Action},
};

APP_STATE_MACHINE_DEF(p_change_state
						,sizeof(jumpStateAction0000)/sizeof(jumpStateAction0000[0])
						,&jumpStateAction0000[0]);

static StateNumber t00000Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); mStateMachineStateSet(p_change_state,4);return 1;}
static StateNumber t00001Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); mStateMachineStateSet(p_change_state,2);return 2;}
static StateNumber t00002Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); mStateMachineStateSet(p_change_state,3);return 3;}
static StateNumber t00003Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); mStateMachineStateSet(p_change_state,0); return 4;}
static StateNumber t00004Action(s_StateMachineParam *p_this){UNUSED_VARIABLE(p_this); mStateMachineStateSet(p_change_state,1);return 0;}

static X_Void SuddenChangeJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	 EXPECT_EQ(state_going_to_enter,mocker_sf.ExpectCurrentState4());
}

TEST(state_machine,state_sudden_change)
{
	uint8_t i;
	EXPECT_CALL(mocker_sf, ExpectCurrentState4()).Times(5).WillOnce(Return(0)).WillOnce(Return(4)).WillOnce(Return(1))
														  .WillOnce(Return(2)).WillOnce(Return(3));

	mStateMachineStateSet(p_change_state,3);
	for(i=0;i<5;i++)
	{
		mStateMachineRun(p_change_state,&sPE.base,SuddenChangeJumpRecorder);
	}
}






GTEST_API_ int main(int argc, char **argv)
{
	cout<<"state machine test from here \r\n"<<endl;
	 testing::InitGoogleTest(&argc, argv);
  	return RUN_ALL_TESTS();
}
