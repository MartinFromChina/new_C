#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../rw_by_line.h"

//#include "stdio.h"
static char context[MAX_LENGTH_OF_ONE_LINE];
//static uint16_t length;

TEST(file,open_and_read)
{
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("data_r.txt");
	isOk = ReadFileByLine("data_r.txt",0,context);
	EXPECT_STREQ("11\r\n",context);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ("2222\r\n",context);

	isOk = ReadFileByLine(p_file_name,2,context);
	EXPECT_STREQ("33433333\r\n",context);

	isOk = ReadFileByLine(p_file_name,3,context);
	EXPECT_STREQ("\r\n",context);

	isOk = ReadFileByLine(p_file_name,4,context);
	EXPECT_STREQ("safasfopiasjiopfgjsapofpowerk ",context);

	isOk = ReadFileByLine(p_file_name,5,context);
	//printf(context);
	EXPECT_EQ(isOk,X_False);

}

TEST(file,open_and_read_random)
{
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("data_r.txt");

	
	isOk = ReadFileByLine(p_file_name,3,context);
	EXPECT_STREQ("\r\n",context);

	isOk = ReadFileByLine(p_file_name,0,context);
	EXPECT_STREQ("11\r\n",context);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ("2222\r\n",context);

	isOk = ReadFileByLine(p_file_name,5,context);
	//printf(context);
	EXPECT_EQ(isOk,X_False);

	isOk = ReadFileByLine(p_file_name,4,context);
	EXPECT_STREQ("safasfopiasjiopfgjsapofpowerk ",context);

	isOk = ReadFileByLine(p_file_name,2,context);
	EXPECT_STREQ("33433333\r\n",context);

}


static char Table[6][100] = {
		"unknow",
		"unsupport",
		"gGattProcExchangeMtu_c"                        , /*!< MTU Exchange */
	    "gGattProcDiscoverAllPrimaryServices_c"        , /*!< Primary Service Discovery */
	    "gGattProcDiscoverPrimaryServicesByUuid_c"       , /*!< Discovery of Services by UUID */
	    "gGattProcFindIncludedServices_c"       , /*!< Discovery of Included Services within a Service range */
};     

TEST(file,open_and_write)
{
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/data_w.txt");
	
	isOk = WriteFileByLine(p_file_name,0,&Table[0][0]);
	ReadFileByLine(p_file_name,0,context);
	EXPECT_STREQ(&Table[0][0],context);
}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running file_operation_test from test_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

