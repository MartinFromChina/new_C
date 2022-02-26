#include "test_common.h"
#include <iostream>
using namespace std;

#include "../wav_to_c.h"
#include "../../../../module/common/FileOperation/rw_by_line.h"
#include "../../../../module/common/InsertLog/InsertLogDebug.h"


TEST(main_func,run)
{
	int err;
	err = test_main_func();
	EXPECT_EQ(err,0);
}

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------wav to c from wav_to_c_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
