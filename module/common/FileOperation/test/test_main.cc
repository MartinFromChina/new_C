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
TEST(file,open_and_read_empty)
{

}


static char Table[6][100] = {
		"0_aa",
		"1_bbb",
		"2_ccc"   ,               
	    "3_ddd"        , 
	    "4_eee"       , 
	    "5_fff"       ,
};     
/*
TEST(file,open_and_write)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/data_w.txt");
	isOk = FileClear(p_file_name);
	EXPECT_EQ(isOk,X_True);
	
	for(i=0;i<6;i++)
	{
		isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
		ReadFileByLine(p_file_name,i,context);
		//EXPECT_STREQ(strcat(&Table[i][0],"\r\n"),context);
		EXPECT_STREQ(&Table[i][0],context);
	}
	
}

TEST(file,open_and_write_random_simple)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/data_w.txt");
	isOk = FileClear(p_file_name);
	EXPECT_EQ(isOk,X_True);
	
	i = 2;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);


	i ++;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);
	
	//isOk = ReadFileByLine(p_file_name,5,context);
	//EXPECT_EQ(isOk,X_True);
	//EXPECT_STREQ(strcat(&Table[5][0],"\n"),context);

		
    i ++;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);

	i ++;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);
	

	i = 3;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\r\n"),context);

	i = 1;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\r\n"),context);
	
}

static char Table1[6][100] = {
		"0_aa",
		"1_bbb",
		"2_ccc"   ,               
	    "3_ddd"        , 
	    "4_eee"       , 
	    "5_fff"       ,
};     

TEST(file,open_and_write_random_complex)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/data_w.txt");
	isOk = FileClear(p_file_name);
	EXPECT_EQ(isOk,X_True);
	
	i = 5;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);


	i = 4;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);
	
	//isOk = ReadFileByLine(p_file_name,5,context);
	//EXPECT_EQ(isOk,X_True);
	//EXPECT_STREQ(strcat(&Table[5][0],"\n"),context);

		
    i = 2;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);

	i = 3;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);
	

	i = 3;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\r\n"),context);

	i = 1;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	ReadFileByLine(p_file_name,i,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\r\n"),context);
	
}
*/
TEST(file,open_and_write_random_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/empty/empty.txt");

	i = 5;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
}
TEST(file,open_and_write_random_first_line_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/FirstLineEmpty/first_empty.txt");
	
	i = 5;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
}
TEST(file,open_and_write_random_empty_middle_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/MiddleLineEmpty/middle_empty.txt");

	i = 5;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
}
TEST(file,open_and_write_random_full)
{
  
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name;
	p_file_name = ConvFileStrToChar("./write_data/full/full.txt");
	
	i = 5;
	isOk = WriteFileByLine(p_file_name,i,&Table[i][0]);
	
}




GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running file_operation_test from test_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  testing::FLAGS_gtest_filter = "file.open_and_write_random*";
  return RUN_ALL_TESTS();
}

