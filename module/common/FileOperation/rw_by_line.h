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

char* ConvFileStrToChar(const char *src);
X_Boolean WriteFileByLine(const char* p_filename,uint16_t line_num,const char *p_string,...); // linux sz : send Zmodem
X_Boolean ReadFileByLine(const char* p_filename,uint16_t line_num,char *p_context);


#ifdef __cplusplus
		}
#endif

#endif
