#ifndef __RW_BY_LINE_H
#define __RW_BY_LINE_H

#ifdef __cplusplus
	extern "C" {
#endif
#include "../x_cross_platform.h"
#include "../CommonMarco.h"


X_Void WriteFileByLine(const char* p_filename,const char *p_string,...); // linux sz : send Zmodem
uint16_t ReadFileByLine(const char* p_filename,char *p_context,uint16_t *p_length);


#ifdef __cplusplus
		}
#endif

#endif
