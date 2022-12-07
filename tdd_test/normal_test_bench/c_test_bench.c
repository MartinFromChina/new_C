#include "test_bench.h"
#include <string.h>
#include <stdio.h>
#include "../../module/common/AppCommon.h"
#include "../../module/common/InsertLog/InsertLogDebug.h"

void c_tb_selfintroduce(void)
{
    printf("hello , I am c test bench \r\n");
}

/**********************************************add the test code below************************************************/
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
        if(cur_version = 0xffff)
        {
            min_index = i;
            cur_info.total_length = 0; // no valid files;
            break;
        }
        if(cur_version < version_min)
        {
            version_min = cur_version; 
            min_index = i;
        }
    }
    
    byteBufInit(cur_info.file_name,MAX_FILE_NAME_LENGTH,'\0');
    CopyBuffer(p_file_name,cur_info.file_name,length);
    cur_info.start_addr = addr_table[file_type][min_index];
    cur_info.cur_pointer = 0;
    cur_info.isValid = X_True;
    
    return &cur_info;
}


X_Void File_Close(sFileInfo * p_file) 
{
    if(p_file == INVALID_FILE_INFO) {return;}
    p_file ->isValid = X_False;
}