#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include <iostream>
using namespace std;
#include "../rw_by_line.h"

//#include "stdio.h"
static char context[100];
//static uint16_t length;
TEST(file,open)
{
	X_Boolean isOk;
	char buf[100];
	strcpy(buf, "data_r.txt");

	isOk = ReadFileByLine(buf,0,context);
	EXPECT_STREQ("11\r\n",context);

	isOk = ReadFileByLine(buf,1,context);
	EXPECT_STREQ("2222\r\n",context);

	isOk = ReadFileByLine(buf,2,context);
	EXPECT_STREQ("33433333\r\n",context);

	isOk = ReadFileByLine(buf,3,context);
	EXPECT_STREQ("\r\n",context);

	isOk = ReadFileByLine(buf,4,context);
	EXPECT_STREQ("safasfopiasjiopfgjsapofpowerk ",context);

	isOk = ReadFileByLine(buf,5,context);
	//printf(context);
	EXPECT_EQ(isOk,X_False);

}

GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running file_operation_test from test_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

