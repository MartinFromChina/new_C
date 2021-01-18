#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../InsertLogDebug.h"

#define USE_INSERT_DEBUG 1
//#define LOG_DEBUG_METHOD  printf


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

#if (DEBUG_METHOD == log_printf)
/*
static X_Boolean  mLog_method_init(log_func *pp_func_reg)
{
	if(pp_func_reg == X_Null) {return X_False;}
	else {*pp_func_reg = printf;return X_True;}
}
*/
#endif


#if (USE_INSERT_DEBUG != 0)
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,1);
INSERT(LOG_COUNTER_ENTRY_DEF)(p_counter,0);


TEST(insert_LOG_DEBUG,printf)
{
	uint8_t i;
	//insert_LogDebugInit(mLog_method_init);
	//insert_LOG_Debug
	//cout<<"------------ should print  once counter1 counter2\r\n";
	INSERT(LogDebug)(1,("hello \r\n"));
	INSERT(LogDebug)(0,("!!!!mistake 1\r\n"));

	INSERT(LogDebugOnce)(1,p_once,5,("once \r\n"));
	INSERT(LogDebugCounter)(1,p_counter,10,("counter 1\r\n")); 

	for(i=0;i<9;i++)
	{
		INSERT(LogDebugOnce)(1,p_once,5,("!!!!mistake 2\r\n"));
		INSERT(LogDebugCounter)(1,p_counter,10,("!!!!mistake 3\r\n"));
	}
	INSERT(LogDebugCounter)(1,p_counter,10,("counter 2\r\n"));
}
#else
TEST(remove_LOG_DEBUG,init)
{
	cout<<"------------same as TEST(insert_LOG_DEBUG,printf),but nothing print \r\n";

	uint8_t i;
	//insert_LogDebugInit(mLog_method_init);
	//insert_LOG_Debug
	INSERT(LogDebug)(1,("hello \r\n"));
	INSERT(LogDebug)(0,("!!!!mistake 1\r\n"));

	INSERT(LogDebugOnce)(1,p_once,5,("once \r\n"));
	INSERT(LogDebugCounter)(1,p_counter,10,("counter 1\r\n")); 

	for(i=0;i<9;i++)
	{
		INSERT(LogDebugOnce)(1,p_once,5,("!!!!mistake 2\r\n"));
		INSERT(LogDebugCounter)(1,p_counter,10,("!!!!mistake 3\r\n"));
	}
	INSERT(LogDebugCounter)(1,p_counter,10,("counter 2\r\n"));
}

#endif


GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------insert_log_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
