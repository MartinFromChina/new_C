#ifndef __RW_BY_LINE_H
#define __RW_BY_LINE_H

#ifdef __cplusplus
	extern "C" {
#endif
#include "../x_cross_platform.h"
#include "../CommonMarco.h"

#ifndef MAX_LENGTH_OF_ONE_LINE
#define MAX_LENGTH_OF_ONE_LINE  500
#endif

#ifndef MAX_LENGTH_OF_FILE_NAME
#define MAX_LENGTH_OF_FILE_NAME  256
#endif

#ifndef TOTAL_LINE_NUM_MAX
#define TOTAL_LINE_NUM_MAX  1000
#endif


char* ConvFileStrToChar(const char *src,char *p_buf);
X_Boolean FileClear(const char* p_filename);
X_Boolean WriteFileByLine(const char* p_filename,uint16_t line_num,const char *p_string,...); // linux sz : send Zmodem
X_Boolean ReadFileByLine(const char* p_filename,uint16_t line_num,char *p_context);

/*
this function is just for test,not stable
*/
X_Boolean CompareTwoFileByLine(const char* p_filename1,const char* p_filename2,uint16_t start_line,uint16_t end_line);


#ifdef __cplusplus
		}
#endif

#endif
