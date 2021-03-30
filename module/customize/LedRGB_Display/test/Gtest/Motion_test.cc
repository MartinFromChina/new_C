#include "test_common.h"
#include <iostream>
using namespace std;

#include "../../../../../module/common/FileOperation/rw_by_line.h"
#include "../../../../../module/common/InsertLog/InsertLogDebug.h"
#include "../../c/finally.h"
/*
X_Void SleepMilliseconds(uint32_t ms) {
	uint32_t end = clock() + ms;
	while(clock() < end) 
	{

	}
}
*/

TEST(motion,getAix)
{
	EXPECT_EQ(0, GetX_Xita());
}

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------motion_test from motion_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
