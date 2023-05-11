#include "test_common.h"
#include <iostream>
using namespace std;

#include "../test_bench.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"
#include "../../../module/common/AppCommon.h"

TEST(self,introduce)
{
  /*
	int err;
	err = test_main_func();
	EXPECT_EQ(err,0);
  */
  c_tb_selfintroduce();
  cpp_tb_selfintroduce();
}

/**********************************************add the test code below************************************************/
extern X_Void GeneratorCMD(X_Void);
extern X_Void GeneratorCMDTiming(X_Void);
extern X_Void GeneratorCMDTrigTiming(X_Void);
extern X_Void GeneratorCMD_NeighborTrigTiming_LocalTrigTiming(X_Void);
extern X_Void GeneratorCMD_All(X_Void);
extern X_Void GeneratorCMD_All_2(X_Void);
TEST(cmd,generator)
{
	//GeneratorCMD();
  //GeneratorCMDTiming();
  GeneratorCMDTrigTiming();
  GeneratorCMD_NeighborTrigTiming_LocalTrigTiming();
  GeneratorCMD_All();
  GeneratorCMD_All_2();
}
/************************************************************************************************************************/

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------wav to c from wav_to_c_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}