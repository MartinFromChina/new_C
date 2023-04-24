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



/****c code below ***/
#define  SYS_DEBUG_METHOD printf

#define SysLogDebug(flag,message)        do{ 							\
                                                if((flag) != 0)								\
                                                {	\
                                                        SYS_DEBUG_METHOD	message ;	\
                                                }	\
                                            }while(0)

static X_Boolean isDisplayLog = X_False;
X_Void EnableLog(X_Void)
{
    isDisplayLog = X_True;
}
X_Void DisableLog(X_Void)
{ 
    isDisplayLog = X_False;
}
static int32_t FileStrToIntFix(char *_pStr, uint8_t _ucLen)
{
	uint8_t flag;
	char *p;
	uint32_t ulInt;
	uint8_t i;
	uint8_t ucTemp;

	p = _pStr;
	if (*p == '-')
	{
		flag = 1;	/* 负数 */
		p++;
		_ucLen--;
	}
	else
	{
		flag = 0;
	}

	ulInt = 0;
	for (i = 0; i < _ucLen; i++)
	{
		ucTemp = *p;
		if (ucTemp == '.')	/* 遇到小数点，自动跳过1个字节 */
		{
			p++;
			ucTemp = *p;
		}
		if ((ucTemp >= '0') && (ucTemp <= '9'))
		{
			ulInt = ulInt * 10 + (ucTemp - '0');
			p++;
		}
		else
		{
			break;
		}
	}

	if (flag == 1)
	{
		return -ulInt;
	}
	return ulInt;
}

#define MAX_FILE_NAME_LENGTH  16
uint16_t GetVersionNum(char *p_version_string)
{
    uint16_t number = 0;
    X_Boolean isError = X_False;
    uint8_t major_version = 0xff,i = 0,j = 0;
    uint8_t sec_version_h4 = 0xff,sec_version_l4 = 0xff;
    if(p_version_string == (char *)0) {return 0;}
    for(i=0;i<MAX_FILE_NAME_LENGTH;i++)
    {
        if(major_version == 0xff)
        {
            if(p_version_string[i] == 'V')
            {
                j = i;
            }
            else if(p_version_string[i] == '.')
            {
                if(j == 0 || (i - j) < 2)
                {
                    isError = X_True;
                    break;
                }
                major_version = FileStrToIntFix(&p_version_string[j+1],((i - j) > 0) ? i - j - 1 : 0);
                SysLogDebug(0,("string is %s major_version %d i %d j %d \r\n",p_version_string,major_version,i,j));
                number ++;
                j = i;
            }
        }
        else if(sec_version_h4 == 0xff)
        {
            if(p_version_string[i] == '.')
            {
                if((j + 1) == i)
                {
                    isError = X_True;
                    break;
                }
                sec_version_h4 = FileStrToIntFix(&p_version_string[j+1],((i - j) > 0) ? i - j - 1 : 0);
                SysLogDebug(0,("string is %s sec_version_h4 %d i %d j %d \r\n",p_version_string,sec_version_h4,i,j));
                
                if(sec_version_h4 >= 10)
                {
                    isError = X_True;
                    break;
                }
                number ++;
                j = i;
            }
        }
        else if(sec_version_l4 == 0xff)
        {
            sec_version_l4 = FileStrToIntFix(&p_version_string[j+1],strlen(&p_version_string[j+1]));
            if(sec_version_l4 >= 10)
            {
                isError = X_True;
                break;
            }
            SysLogDebug(0,("string is %s sec_version_l4 %d i %d j %d \r\n",p_version_string,sec_version_l4,i,j));
            number ++;
        }
    }

    if(isError == X_False && number == 3)
    {
        number = (uint16_t)((major_version << 8) & 0xFF00);
        number += (sec_version_h4*10) + sec_version_l4;
    }
    else
    {   
        number = 0xffff;
    }
    SysLogDebug(0,("version %2x.%2x.%2x ; number %2x \r\n",
            major_version,sec_version_h4,sec_version_l4,number));
   
    return number;
}

#define TOTAL_FILE_TYPE_NUMBER           4   // wave; master; relay; FPGA;   
#define MAX_VERSION_FOR_ONE_TYPE_FILE    3   // for example : wave 1.1.1   wave 1.1.2  wave 1.1.3 

typedef enum{
    CFT_relay = 0,
    CFT_master,
    CFT_wave,
    CFT_fpga,
}eCompressedFileType;

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

static sFileInfo cur_info = {
    X_False,
    {"no name"},
    0,0,0,0,
};

#define INVALID_FILE_INFO    ((sFileInfo *)0)

static const uint32_t addr_table[TOTAL_FILE_TYPE_NUMBER][MAX_VERSION_FOR_ONE_TYPE_FILE] = {
    {0,         0x32000,    0x64000},
    {0x96000,   0xC8000,    0xFA000},
    {0x12C000,  0x15E000,   0x190000},
    {0x1C2000,  0x28A000,   0x352000},
};

extern X_Void mockable_mFun_ExtFlashReadBuffer(uint8_t * p_buf, uint32_t start_addr, uint32_t length_in_byte);
extern X_Boolean mockable_mFun_ExtFlashWriteBuffer(uint8_t* p_buf, uint32_t start_addr, uint16_t length_in_byte);
sFileInfo *File_Open(char * p_file_name)
{
    unsigned int length;
    sFileInfo    temp_info;
    uint16_t     cur_version,version_min = 0xffff,min_index = 0;
    eCompressedFileType file_type = CFT_relay;
    if(p_file_name == (char *)0) {return INVALID_FILE_INFO;}
    length = strlen(p_file_name);
    if(length > MAX_FILE_NAME_LENGTH) {return INVALID_FILE_INFO;}
    if(cur_info.isValid == X_True)
    {
        SysLogDebug(1,("!!! fatal error , close file first\r\n"));
        return INVALID_FILE_INFO;
    }

    if(memcmp("MCU",p_file_name,3) == 0)
    {
        switch(p_file_name[3])
        {
            case 'r':
                file_type = CFT_relay;
                break;
            case 'm':
                file_type = CFT_master;
                break;
            case 'w':
                file_type = CFT_wave;
                break;
            default:
                    return INVALID_FILE_INFO;
                ////break;
        }
    }
    else if(memcmp("FPGA",p_file_name,4) == 0)
    {
        file_type = CFT_fpga;
    }
    else
    {
        return INVALID_FILE_INFO;
    }

    for(uint16_t i = 0; i< MAX_VERSION_FOR_ONE_TYPE_FILE; i++ )
    {
        MOCKABLE(mFun_ExtFlashReadBuffer)((uint8_t *)&temp_info,addr_table[file_type][i],sizeof(temp_info)/sizeof(uint8_t));
        cur_version = GetVersionNum((char *)temp_info.file_name);
        if(cur_version == 0xffff)
        {
            min_index = i;
            break;
        }
        if(cur_version < version_min)
        {
            version_min = cur_version; 
            min_index = i;
        }
    }

    MOCKABLE(mFun_ExtFlashReadBuffer)((uint8_t *)&temp_info,addr_table[file_type][min_index],sizeof(temp_info)/sizeof(uint8_t));
    
    //SysLogDebug(1,(" ~~~~~~~~~~~~~ file type %d , min index %d ; crc %2x \r\n",file_type,min_index,temp_info.crc));
    
    byteBufInit(temp_info.file_name,MAX_FILE_NAME_LENGTH,'\0');
    CopyBuffer(p_file_name,temp_info.file_name,length);
    temp_info.cur_pointer = 0;
    temp_info.total_length = 0;
    temp_info.start_addr  = addr_table[file_type][min_index];
    temp_info.isValid     = X_True;
    
    CopyBuffer(&temp_info,&cur_info,sizeof(temp_info)/sizeof(uint8_t));
    
    return &cur_info;
}


X_Void File_Close(sFileInfo * p_file) 
{
    if(p_file == INVALID_FILE_INFO) {return;}
    p_file ->isValid = X_False;

    MOCKABLE(mFun_ExtFlashWriteBuffer)((uint8_t *)p_file,p_file ->start_addr,sizeof(sFileInfo)/sizeof(uint8_t));
}