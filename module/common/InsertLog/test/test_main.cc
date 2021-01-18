#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../InsertLogDebug.h"

#define USE_INSERT_DEBUG 1

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
static X_Boolean  mLog_method_init(log_func *pp_func_reg)
{
	if(pp_func_reg == X_Null) {return X_False;}
	else {*pp_func_reg = printf;return X_True;}
}
*/



#if (USE_INSERT_DEBUG != 0)
TEST(insert_LOG_DEBUG,init)
{
	//insert_LogDebugInit(mLog_method_init);
}
#else
TEST(remove_LOG_DEBUG,init)
{

}

#endif


GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------insert_log_test from test_main.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
