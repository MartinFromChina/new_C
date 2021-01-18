#ifndef __RW_BY_LINE_H
#define __RW_BY_LINE_H

#ifdef __cplusplus
	extern "C" {
#endif
#include "../x_cross_platform.h"
#include "../CommonMarco.h"

#ifndef MAX_LENGTH_OF_LINE
#define MAX_LENGTH_OF_LINE  500
#endif

X_Boolean WriteFileByLine(const char* p_filename,const char *p_string,...); // linux sz : send Zmodem
X_Boolean ReadFileByLine(const char* p_filename,uint16_t line_num,char *p_context);


#ifdef __cplusplus
		}
#endif

#endif
