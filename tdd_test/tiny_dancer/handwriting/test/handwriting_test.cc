#include "test_common.h"
#include <iostream>
using namespace std;

#include "../z_math.h"
#include "../../../../module/common/FileOperation/rw_by_line.h"


TEST(pentip,init)
{
	uint16_t i;
	
	//EXPECT_GT(result_sum, 0.49);
	//EXPECT_LT(result_sum, 0.51);

}

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------handwriting_test from handwriting_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
