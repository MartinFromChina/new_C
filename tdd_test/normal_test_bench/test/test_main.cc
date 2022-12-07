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
static char test_string1[] = "MCUwV1.2.3";
static char test_string2[] = "MCUw1.2.3";  // error 
static char test_string3[] = "MCUwV1.2.11";
static char test_string4[] = "MCUwV1..3";
static char test_string5[] = "MCUwV.2.3";
static char test_string6[] = "MCUwV..3";
static char test_string7[] = "MCUwV1.200.3";

static char test_string8[] = "MCUwV20.9.9";
static char test_string9[] = "FPGAV0.0.1";
static char test_string10[] = "MCUwV11.0.2";
static char test_string11[] = "MCUwV200.0.0";

extern uint16_t GetVersionNum(char *p_version_string);
extern X_Void EnableLog(X_Void);
extern X_Void DisableLog(X_Void);

TEST(get,version_num)
{
	uint16_t number;
	char temp_buf[16];
	DisableLog();

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string1,temp_buf,strlen(test_string1));

	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,(0x100 + 23));
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string2,temp_buf,strlen(test_string2));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string3,temp_buf,strlen(test_string3));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string4,temp_buf,strlen(test_string4));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string5,temp_buf,strlen(test_string5));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string6,temp_buf,strlen(test_string6));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string7,temp_buf,strlen(test_string7));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xffff);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string8,temp_buf,strlen(test_string8));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0x1400 + 99); // 20.9.9

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string9,temp_buf,strlen(test_string9));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,1); // V0.0.1

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string10,temp_buf,strlen(test_string10));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0x0b00 + 2); // 11.0.2

	byteBufInit((uint8_t *)temp_buf,16,'\0');
	CopyBuffer(test_string11,temp_buf,strlen(test_string11));
	//EnableLog();
	number = GetVersionNum(temp_buf);
	EXPECT_EQ(number,0xc800 + 0); // 200.0.0

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}

#define TOTAL_FILE_TYPE_NUMBER           4   // wave; master; relay; FPGA;   
#define MAX_VERSION_FOR_ONE_TYPE_FILE    3   // for example : wave 1.1.1   wave 1.1.2  wave 1.1.3 

typedef enum{
    CFT_relay = 0,
    CFT_master,
    CFT_wave,
    CFT_fpga,
}eCompressedFileType;

#define MAX_FILE_NAME_LENGTH 16

typedef struct
{
    /*****read it from flash*****/
    X_Boolean isValid;
    uint8_t file_name[MAX_FILE_NAME_LENGTH];
    uint32_t total_length;
    uint16_t crc;
    /****************************/
    uint32_t start_addr;
    uint32_t cur_pointer;
}sFileInfo;

#define INVALID_FILE_INFO    ((sFileInfo *)0)

extern sFileInfo *File_Open(char * p_file_name);
extern X_Void File_Close(sFileInfo * p_file);

static char error_string1[] = "UnknowV1.2.3";
static char error_string2[] = "MCUkV1.2.3";

static char open_string1_1[] = "MCUwV1.2.3";
static char open_string1_2[] = "MCUwV2.2.3";
static char open_string1_3[] = "MCUwV2.4.3";
static char open_string1_4[] = "MCUwV2.4.5";

static char open_string2_1[] = "MCUrV0.0.1";
static char open_string2_2[] = "MCUrV1.9.9";
static char open_string2_3[] = "MCUrV15.2.3";
static char open_string2_4[] = "MCUrV16.2.3";

static char open_string3_1[] = "MCUmV9.9.9";
static char open_string3_2[] = "MCUmV10.9.9";
static char open_string3_3[] = "MCUmV11.0.0";
static char open_string3_4[] = "MCUmV12.2.3";

static char open_string4_1[] = "FPGAV1.2.3";
static char open_string4_2[] = "FPGAV1.2.3";
static char open_string4_3[] = "FPGAV9.2.3";
static char open_string4_4[] = "FPGAV4.0.0";

static const uint32_t addr_table[TOTAL_FILE_TYPE_NUMBER][MAX_VERSION_FOR_ONE_TYPE_FILE] = {
    {0,         0x32000,    0x64000}, // relay 
    {0x96000,   0xC8000,    0xFA000}, // master
    {0x12C000,  0x15E000,   0x190000},// wave
    {0x1C2000,  0x28A000,   0x352000},// FPGA
};

X_Void mockable_mFun_ExtFlashReadBuffer(uint8_t * p_buf, uint32_t start_addr, uint32_t length_in_byte)
{
	
}

TEST(open,close)
{
	sFileInfo * cur_file = INVALID_FILE_INFO;

	cur_file = File_Open(error_string1);
	EXPECT_EQ(cur_file,INVALID_FILE_INFO);
	

	cur_file = File_Open(error_string2);
	EXPECT_EQ(cur_file,INVALID_FILE_INFO);


	cur_file = File_Open(open_string1_1);
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x12C000,cur_file->start_addr);
	cur_file = INVALID_FILE_INFO;

}
/************************************************************************************************************************/

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------wav to c from wav_to_c_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
