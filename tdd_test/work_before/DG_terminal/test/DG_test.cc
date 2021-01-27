#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"

#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include <iostream>
#include "../../../../module/common/InsertLog/InsertLogDebug.h"

using namespace std;

TEST(node,init)
{
}



GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------DG_mul_terminal_test from DG_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "trans.init";
  return RUN_ALL_TESTS();
}



