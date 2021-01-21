#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../ProtocolRecv.h"


#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

/*

#if (SEGGER_RTT_DEBUG == 0)
	#ifdef DEBUG 
		#error [will take the risk that marco "DEBUG" cause hardfault in product !!!]
	#endif
#else 
	#ifndef DEBUG 
		#warning [will miss more detail when fatal error occur if disable marco "DEBUG"]
	#endif
#endif

*/
static X_DATA_UNIT empty_unit_receive(X_Void){return 0;}
X_Boolean empty_find_header(X_DATA_UNIT current_data,X_DATA_UNIT *p_header_buf,uint16_t *p_header_length){return X_True;}
e_find_other_process empty_find_others(X_DATA_UNIT current_data,X_DATA_UNIT *p_other_buf,uint16_t *p_other_unit){return FOP_successed;}


TEST(Protocol_recv,init)
{
	find_others_and_checksum 	p3 = (find_others_and_checksum)0;
	find_header 				p2 = (find_header)0;
	unit_receive 				p1 = (unit_receive)0;

	sProtocolRecv * p_uart0 = (sProtocolRecv *)0;
	ProtocolRecvInit(p_uart0);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart0),X_False);
	
	PROTOCOL_RECV_DATA_BUF_DEF(p_uart1,MAX_FRAME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart1);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart1),X_True);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart2,MAX_FRAME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,p3);
	ProtocolRecvInit(p_uart2);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart2),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart3,MAX_FRAME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,p2,empty_find_others);
	ProtocolRecvInit(p_uart3);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart3),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart4,MAX_FRAME_LENGTH,MAX_FRAME_CHCHE_NUM,p1,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart4);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart4),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart5,MAX_FRAME_LENGTH+1,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart5);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart5),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart6,MAX_FRAME_LENGTH,MAX_FRAME_CHCHE_NUM+1,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart6);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart6),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart7,MAX_FRAME_LENGTH,0,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart7);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart7),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart8,0,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart8);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart8),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart9,10,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart9),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart10,10,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart10);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart10),X_True);
	
}



#include "../../../common/StateMachine/StateMachine.h"
#include "../../../common/FileOperation/rw_by_line.h"
#include "../../../common/CharToNum/char_to_num.h"



typedef X_Void (*func_irq)(X_Void);
typedef X_Void (*func_main_loop)(X_Void);


typedef struct
{
	s_StateMachineParam 				base;
	uint16_t							max_valid_size;
	uint16_t                            irq_freq;
	uint16_t                            mian_loop_freq;
	uint16_t 							wait_counter;
	X_Boolean 							isFinished;
	func_irq							irq;
	func_main_loop						main_loop;
}sParamExtern;

static sParamExtern sPE;

typedef enum
{
	CTI_Idle = 0,
	CTI_Irq ,
	CTI_Main_loop,
	CTI_End ,
}ConvTXT_to_irq_state;

/**********************************************************************************************************************************
			0
*********************************************************************************************************************************/
static StateNumber CTI_IdleAction(s_StateMachineParam *p_this)
{
	sParamExtern * p_backup = (sParamExtern*)p_this;
	p_backup ->isFinished = X_True;
	return p_this->current_state;
}
static StateNumber CTI_IrqAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CTI_Main_loopAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}
static StateNumber CTI_EndAction(s_StateMachineParam *p_this)
{
	return p_this->current_state;
}




static const StateAction CTI_StateAction[] = {
		{CTI_IdleAction},
		{CTI_IrqAction},
		{CTI_Main_loopAction},
		{CTI_EndAction},
};

APP_STATE_MACHINE_DEF(p_state
								,sizeof(CTI_StateAction)/sizeof(CTI_StateAction[0])
								,&CTI_StateAction[0]);

static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter)
{
	
}

static X_Void TestBench(X_Void)
{
	while(sPE.isFinished = X_False)
	{
		mStateMachineRun(p_state,&sPE.base,StateJumpRecorder);
	}
}

static uint8_t base_data_buf[1000];

static X_Void TestBenchInit(uint16_t irq_freq,uint16_t main_freq
	/*,func_irq irq,func_main_loop main_loop*/
	,const char * p_file_path
	)
{
	uint16_t i;
	char buf[MAX_LENGTH_OF_ONE_LINE],name_buf[200];
	sPE.isFinished 		= X_False;
	sPE.wait_counter 	= 0;
	sPE.irq_freq 		= irq_freq;
	sPE.mian_loop_freq  = main_freq;
	//sPE.irq 			= irq;
	//sPE.main_loop       = main_loop;

	mStateMachineStateSet(p_state,CTI_Idle);

	for(i=0;i<1000;i++){base_data_buf[i] = 0xFF;}

	char *p_file_name = ConvFileStrToChar(p_file_path,name_buf);

	uint8_t line_num = 0,_8_bit;
	uint16_t length,j,k;
	X_Boolean isOK = X_True,isConv;
	j = 0;
	for(line_num = 0;isOK == X_True;line_num ++)
	{
		isOK = ReadFileByLine(p_file_name,line_num,buf);
		//if(buf[0] == '\n') {break;}
		
		length = GetStringLength(buf);
		printf("length %d ;%s",length,buf);
		if((length % 2) != 0) {printf("error !!!;length %d ;%s",length,buf);return;}
		
		for(i = 0,k = 0;i<(length);i = i + 2,k++)
		{
			_8_bit = HexCharTo_8bit(buf[i],buf[i+1],&isConv);
			base_data_buf[j + k] = _8_bit;
			//printf(" %2x load to base_data_buf[%d] \r\n",_8_bit,j + k);
		}
		j += (length/2);
	}
	sPE.max_valid_size = j;
/*
	for(i=0;i<j;i++)
	{
		printf(" %2x ",base_data_buf[i]);
	}
	printf("\r\n");
	*/
	
}
	TEST(CharToNum,_8bit)
	{
		X_Boolean isOk;
		char buf[13] = {
			'0','5',
			'9','8',
			'4','0',
			'f','f',
			'f','e',
			'0','0',
			'\n'
		};

		EXPECT_EQ( GetStringLength(buf), 12); 
		
		EXPECT_EQ( HexCharTo_8bit(buf[0],buf[1],&isOk), 5); 
		EXPECT_EQ( HexCharTo_8bit(buf[2],buf[3],&isOk), 0x98); 
		EXPECT_EQ( HexCharTo_8bit(buf[4],buf[5],&isOk), 0x40); 
		EXPECT_EQ( HexCharTo_8bit(buf[6],buf[7],&isOk), 255); 
		EXPECT_EQ( HexCharTo_8bit(buf[8],buf[9],&isOk), 254); 
		EXPECT_EQ( HexCharTo_8bit(buf[10],buf[11],&isOk), 0); 
	}


TEST(Protocol_recv,find_headers)
{
	TestBenchInit(1,1,"./data_recv/data1.txt");
	TestBench();
}

TEST(Protocol_recv,find_whole_frame)
{

}

TEST(Protocol_recv,stress_testing)
{

}

TEST(Protocol_recv,mul_entrys)
{

}




GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------protocol_recv_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
