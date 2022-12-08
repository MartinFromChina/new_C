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

static uint8_t fake_flash[12][200];

X_Void mockable_mFun_ExtFlashReadBuffer(uint8_t * p_buf, uint32_t start_addr, uint32_t length_in_byte)
{
	uint8_t *p_flash = (uint8_t*)0;
	uint16_t index = 0xffff;
	/////printf("start_addr %2x \r\n",start_addr);
	switch(start_addr)
	{
		case 0:
			p_flash = &fake_flash[0][0];
			index   = 0;
			break;
		case 0x32000:
			p_flash = &fake_flash[1][0];
			index   = 1;
			break;
		case 0x64000:
			p_flash = &fake_flash[2][0];
			index   = 2;
			break;
		case 0x96000:
			p_flash = &fake_flash[3][0];
			index   = 3;
			break;
		case 0xC8000:
			p_flash = &fake_flash[4][0];
			index   = 4;
			break;
		case 0xFA000:
			p_flash = &fake_flash[5][0];
			index   = 5;
			break;
		case 0x12C000:
			p_flash = &fake_flash[6][0];
			index   = 6;
			/*
			printf(" ok \r\n");
			for(uint16_t i = 0;i<200;i++)
			{
				printf("%2x",p_flash[i]);
			}
			printf("\r\n");
			*/
			break;
		case 0x15E000:
			p_flash = &fake_flash[7][0];
			index   = 7;
			break;
		case 0x190000:
			p_flash = &fake_flash[8][0];
			index   = 8;
			break;
		case 0x1C2000:
			p_flash = &fake_flash[9][0];
			index   = 9;
			break;
		case 0x28A000:
			p_flash = &fake_flash[10][0];
			index   = 10;
			break;
		case 0x352000:
			p_flash = &fake_flash[11][0];
			index   = 11;
			break;
		default:
			break;
	}

	if(p_flash == (uint8_t*)0 || p_buf == (uint8_t*)0 || length_in_byte > 200 ) 
	{
		printf("fatal error ");
		return;
	}

	CopyBuffer(p_flash,p_buf,length_in_byte);
/*
	printf(" read from index %d , length %d \r\n",index,length_in_byte);
	for(uint16_t i = 0;i<length_in_byte;i++)
	{
		printf("%2x",p_flash[i]);
	}
	printf("\r\n");
*/
}
X_Boolean mockable_mFun_ExtFlashWriteBuffer(uint8_t* p_buf, uint32_t start_addr, uint16_t length_in_byte)
{
	uint8_t *p_flash = (uint8_t*)0;
	uint16_t index = 0xffff;
	switch(start_addr)
	{
		case 0:
			p_flash = &fake_flash[0][0];
			index = 0;
			break;
		case 0x32000:
			p_flash = &fake_flash[1][0];
			index = 1;
			break;
		case 0x64000:
			p_flash = &fake_flash[2][0];
			index = 2;
			break;
		case 0x96000:
			p_flash = &fake_flash[3][0];
			index = 3;
			break;
		case 0xC8000:
			p_flash = &fake_flash[4][0];
			index = 4;
			break;
		case 0xFA000:
			p_flash = &fake_flash[5][0];
			index = 5;
			break;
		case 0x12C000:
			p_flash = &fake_flash[6][0];
			index = 6;
			break;
		case 0x15E000:
			p_flash = &fake_flash[7][0];
			index = 7;
			break;
		case 0x190000:
			p_flash = &fake_flash[8][0];
			index = 8;
			break;
		case 0x1C2000:
			p_flash = &fake_flash[9][0];
			index = 9;
			break;
		case 0x28A000:
			p_flash = &fake_flash[10][0];
			index = 10;
			break;
		case 0x352000:
			p_flash = &fake_flash[11][0];
			index = 11;
			break;
		default:
			break;
	}

	if(p_flash == (uint8_t*)0 || p_buf == (uint8_t*)0 || length_in_byte > 200 ) 
	{
		printf("fatal error ");
		return X_False;
	}

	CopyBuffer(p_buf,p_flash,length_in_byte);

/*
	printf(" write to index %d , length %d \r\n",index,length_in_byte);
	for(uint16_t i = 0;i<length_in_byte;i++)
	{
		printf("%2x",p_buf[i]);
	}
	printf("\r\n");
*/
	return X_True;
}
static X_Void ClearFakeFlash(X_Void)
{
	for(uint8_t i = 0;i<12;i++)
	{
		byteBufInit(&fake_flash[i][0],200,0xff);
	}
}

TEST(open,close)
{
	sFileInfo * cur_file = INVALID_FILE_INFO;
	ClearFakeFlash();

	cur_file = File_Open(error_string1);
	EXPECT_EQ(cur_file,INVALID_FILE_INFO);
	

	cur_file = File_Open(error_string2);
	EXPECT_EQ(cur_file,INVALID_FILE_INFO);

/*~~~~~~~~~~~~~~~~~~~~1 wave ~~~~~~~~~~~~~~~~~~~~~~~*/
/*
	static const uint32_t addr_table[TOTAL_FILE_TYPE_NUMBER][MAX_VERSION_FOR_ONE_TYPE_FILE] = {
    {0,         0x32000,    0x64000}, // relay 
    {0x96000,   0xC8000,    0xFA000}, // master
    {0x12C000,  0x15E000,   0x190000},// wave
    {0x1C2000,  0x28A000,   0x352000},// FPGA
};
*/
	cur_file = File_Open(open_string1_1);
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x12C000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);

	cur_file ->cur_pointer  = 50;
	cur_file ->total_length = 100;
	cur_file ->crc          = 0x1234;
	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


	cur_file = File_Open(open_string1_2);
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x15E000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");

	
	cur_file = File_Open(open_string1_3);
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x190000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


	cur_file = File_Open(open_string1_4);
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x12C000,cur_file->start_addr);
	EXPECT_EQ(0x1234,cur_file ->crc);

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");

/*~~~~~~~~~~~~~~~~~~1 wave again~~~~~~~~~~~~~~~~~~~~~~~~~*/
ClearFakeFlash();

cur_file = File_Open(open_string1_3);     // MCUwV2.4.3
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x12C000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


	cur_file = File_Open(open_string1_1); // 1.2.3
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x15E000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);
	cur_file ->cur_pointer  = 50;
	cur_file ->total_length = 100;
	cur_file ->crc          = 0x1234;

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


	cur_file = File_Open(open_string1_2); // 2.2.3
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x190000,cur_file->start_addr);
	EXPECT_EQ(0xffff,cur_file ->crc);

	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


	cur_file = File_Open(open_string1_4); // 2.4.5
	EXPECT_NE(cur_file,INVALID_FILE_INFO);
	EXPECT_EQ(X_True,cur_file->isValid);
	EXPECT_EQ(0,cur_file->total_length);
	EXPECT_EQ(0x15E000,cur_file->start_addr);
	EXPECT_EQ(0x1234,cur_file ->crc);
	
	File_Close(cur_file);
	cur_file = INVALID_FILE_INFO;
	//printf("\r\n\r\n");


/*~~~~~~~~~~~~~~~~~~2 relay ~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~3 master~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~4 FPGA~~~~~~~~~~~~~~~~~~~~~~~~~*/

}
/*
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
*/
/************************************************************************************************************************/

 GTEST_API_ int main(int argc, char **argv) {
   cout<<"------------wav to c from wav_to_c_test.cc \r\n";
   testing::InitGoogleTest(&argc, argv);
  //testing::FLAGS_gtest_filter = "DG.1_to_3_terminal_2_trans_down";
  
  return RUN_ALL_TESTS();
}
