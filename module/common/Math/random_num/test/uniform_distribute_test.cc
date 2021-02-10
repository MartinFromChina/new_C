#include "../../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;

#include "../uniform_distribution.h"
#include "../../../FileOperation/rw_by_line.h"
#include "../../../InsertLog/InsertLogDebug.h"


TEST(uniform,distribution)
{
	uint8_t i;
	int seed =  13579;
	X_Float64 result,result_sum = 0;
	char temp[320];
 	const char *filename = "./text/display.txt";
	FileClear(filename);
	
	for(i=0;i<100;i++)
	{
		result = UniformGenerater(0.0,1.0,&seed);
		insert_LogDebug(1,("result is %.7f \r\n",result));
		result_sum += result;
		snprintf(temp,320,"%.7f",result);
		WriteFileByLine(filename,i,temp);
	}
	result_sum = result_sum/i;
	insert_LogDebug(1,("average is %.7f \r\n",result_sum));
	EXPECT_GT(result_sum, 0.49);
	EXPECT_LT(result_sum, 0.51);
}
 

 
 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------ramdon_test from uniform_distribution_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}

