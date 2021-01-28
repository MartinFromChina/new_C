#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "test_data.h"
#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

#include <iostream>
#include "../hal.h"
#include "test_data.h"


using namespace std;

TEST(DG,init)
{	
	uint8_t *p_data,length,error_data[6] = {0xaa,0x55,06,0,0,05};
	X_Boolean isOK;
	HAL_BasicSet();
	
	length = GenerateBasicInfSet(&p_data,0xff,1,1,0,0,2,3);
	isOK = SendWaveSetForTestModule(5,0,p_data,length,ED_bidirection,50);
	
	length = GenerateBasicInfGet(&p_data,0xff,1,1);
	isOK = SendWaveSetForTestModule(5,200,p_data,length,ED_bidirection,9);

	isOK = SendWaveSetForTestModule(5,250,error_data,6,ED_bidirection,9); // length bigger than 5 , if not the fun_find_other will not take it
	EXPECT_EQ(isOK, X_True);
	
	HAL_Run();
}



GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------DG_mul_terminal_test from DG_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "trans.init";
  return RUN_ALL_TESTS();
}



