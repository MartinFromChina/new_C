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
	
	PROTOCOL_RECV_DATA_BUF_DEF(p_uart1,MAX_FARME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart1);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart1),X_True);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart2,MAX_FARME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,p3);
	ProtocolRecvInit(p_uart2);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart2),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart3,MAX_FARME_LENGTH,MAX_FRAME_CHCHE_NUM,empty_unit_receive,p2,empty_find_others);
	ProtocolRecvInit(p_uart3);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart3),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart4,MAX_FARME_LENGTH,MAX_FRAME_CHCHE_NUM,p1,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart4);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart4),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart5,MAX_FARME_LENGTH+1,MAX_FRAME_CHCHE_NUM,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart5);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart5),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart6,MAX_FARME_LENGTH,MAX_FRAME_CHCHE_NUM+1,empty_unit_receive,empty_find_header,empty_find_others);
	ProtocolRecvInit(p_uart6);
	EXPECT_EQ(DoesProtocolRecvInitOK(p_uart6),X_False);

	PROTOCOL_RECV_DATA_BUF_DEF(p_uart7,MAX_FARME_LENGTH,0,empty_unit_receive,empty_find_header,empty_find_others);
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


GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------insert_log_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
