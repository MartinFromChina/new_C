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
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./read_data/data_r.txt",buf);
	isOk = ReadFileByLine("./read_data/data_r.txt",0,context);
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
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./read_data/data_r.txt",buf);

	
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
	X_Boolean isOk;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./read_data/data_r_empty.txt",buf);
	isOk = ReadFileByLine("./read_data/data_r_empty.txt",0,context);
	EXPECT_STREQ("\n",context);

	isOk = ReadFileByLine("./read_data/data_r_empty.txt",2,context);
	EXPECT_STREQ("\n",context);

	isOk = ReadFileByLine("./read_data/data_r_empty.txt",1,context);
	EXPECT_STREQ("\n",context);

	isOk = ReadFileByLine("./read_data/data_r_empty.txt",10,context);
	EXPECT_STREQ("\n",context);
}


static char Table[6][100] = {
		"0_aa",
		"1_bbb ",
		"2_cccasdasdfadfsagf../;..,.,",               
	    "3_ddd", 
	    "4_eee", 
	    "5_fff",
};     

TEST(file,open_and_write_random_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./write_data/empty/empty.txt",buf);

	i = 3;
	isOk = WriteFileByLine(p_file_name,20,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,20,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);

	
	isOk = CompareTwoFileByLine(p_file_name,"./write_data/Backup/empty.txt",0,19);
	EXPECT_EQ(isOk,X_True);
	isOk = CompareTwoFileByLine(p_file_name,"./write_data/Backup/empty.txt",0,20);
	EXPECT_EQ(isOk,X_False);

	i = 4;
	isOk = WriteFileByLine(p_file_name,21,&Table[i][0]);
	

	isOk = ReadFileByLine(p_file_name,21,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);

	isOk = ReadFileByLine(p_file_name,2,context);// call this , if not the test case in line 251 will failed , debug it later
	EXPECT_STREQ("\n",context);

	isOk = CompareTwoFileByLine(p_file_name,"./write_data/Backup/empty.txt",0,19);
	EXPECT_EQ(isOk,X_True);

	
	isOk = CompareTwoFileByLine(p_file_name,"./write_data/Backup/empty.txt",0,21);
	EXPECT_EQ(isOk,X_False);


	i = 2;
	isOk = WriteFileByLine(p_file_name,3,&Table[i][0]);
	

	isOk = ReadFileByLine(p_file_name,3,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);

	isOk = ReadFileByLine(p_file_name,20,context);
	EXPECT_STREQ(strcat(&Table[3][0],""),context);

	isOk = ReadFileByLine(p_file_name,21,context);
	EXPECT_STREQ(strcat(&Table[4][0],""),context);
	

	i = 2;
	isOk = WriteFileByLine(p_file_name,0,&Table[i][0]);
	

	isOk = ReadFileByLine(p_file_name,0,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	isOk = ReadFileByLine(p_file_name,3,context); // 
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	isOk = ReadFileByLine(p_file_name,20,context);  // 
	EXPECT_STREQ(strcat(&Table[3][0],""),context);

	isOk = ReadFileByLine(p_file_name,21,context);  // 
	EXPECT_STREQ(strcat(&Table[4][0],""),context);
}
TEST(file,open_and_write_random_first_line_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./write_data/FirstLineEmpty/first_empty.txt",buf);

	isOk = CompareTwoFileByLine(p_file_name,"./write_data/Backup/first_empty.txt",0,7);
	EXPECT_EQ(isOk,X_True);
	
	i = 5;
	isOk = WriteFileByLine(p_file_name,20,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,20,context);
	EXPECT_STREQ(strcat(&Table[i][0],"\n"),context);
	isOk = ReadFileByLine(p_file_name,3,context);
	EXPECT_STREQ("cxdddfsagfsdaffg\n",context);

	i = 3;
	isOk = WriteFileByLine(p_file_name,1,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	i = 2;
	isOk = WriteFileByLine(p_file_name,0,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,0,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ(strcat(&Table[3][0],""),context);

	isOk = ReadFileByLine(p_file_name,20,context);
	EXPECT_STREQ(strcat(&Table[5][0],""),context);
	

}
TEST(file,open_and_write_random_empty_middle_empty)
{
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./write_data/MiddleLineEmpty/middle_empty.txt",buf);

	i = 5;
	isOk = WriteFileByLine(p_file_name,15,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,15,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	i = 3;
	isOk = WriteFileByLine(p_file_name,1,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	i = 2;
	isOk = WriteFileByLine(p_file_name,0,&Table[i][0]);

	isOk = ReadFileByLine(p_file_name,0,context);
	EXPECT_STREQ(strcat(&Table[i][0],""),context);

	isOk = ReadFileByLine(p_file_name,1,context);
	EXPECT_STREQ(strcat(&Table[3][0],""),context);

	isOk = ReadFileByLine(p_file_name,15,context);
	EXPECT_STREQ(strcat(&Table[5][0],""),context);
}
TEST(file,open_and_write_random_full)
{
  
	uint8_t i;
	X_Boolean isOk;
	char *p_file_name,buf[MAX_LENGTH_OF_FILE_NAME];
	p_file_name = ConvFileStrToChar("./write_data/full/full.txt",buf);
	
	i = 5;
	isOk = WriteFileByLine(p_file_name,20,&Table[i][0]);
	
}




GTEST_API_ int main(int argc, char **argv) {
  cout<<"------------Running file_operation_test from test_test.cc \r\n";
  testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "file.open_and_write_random_empty";
  return RUN_ALL_TESTS();
}

