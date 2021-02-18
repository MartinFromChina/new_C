#include "test_common.h"
#include <iostream>
using namespace std;

#include "../pentip_pressure.h"
#include "../../../../module/common/FileOperation/rw_by_line.h"
#include "../../../../module/common/Math/random_num/normal_distribution.h"
#include "../../../../module/common/CharToNum/char_to_num.h"


static uint16_t raw_index = 0;
X_Void TestInit(X_Void)
{
	raw_index = 0;
}

uint16_t mockable_PentipGetRaw(X_Void)
{
	X_Boolean isOK;
	uint16_t data;
	char temp[100];
	ReadFileByLine("./text/gauss.txt",raw_index,temp);
	data = (uint16_t)DecCharTo_32bit(temp,4,&isOK);
	if(raw_index >= SAMPLE_CNT ){raw_index = 0;}
	else {raw_index ++;}
	return data;
}

TEST(pentip,base)
{
	uint16_t i;
	X_Float64 result;
	int seed = 13579;
	char tem[320];
	
	const char *filename = "./text/gauss.txt";
	FileClear(filename);
	
	for(i=0;i<500;i++)
	{
		result = GaussGenerator(1700.0,15.0,&seed);
		snprintf(tem,320,"%.7f",result);
		WriteFileByLine(filename,i,tem);
	}

	//EXPECT_GT(result_sum, 0.49);
	//EXPECT_LT(result_sum, 0.51);

}

 TEST(pentip,init)
 {
 	TestInit();
 	PentipInit(mockable_PentipGetRaw);
 }

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------pressure_test from pressure_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
