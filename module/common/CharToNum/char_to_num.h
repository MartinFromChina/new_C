#ifndef __CHAR_TO_NUM_H
#define __CHAR_TO_NUM_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../../common/x_cross_platform.h"

uint16_t GetStringLength(char *p_strings);
uint8_t HexCharTo_8bit(char hexchar_H,char hexchar_L,X_Boolean *isOK);
uint32_t DecCharTo_32bit(char *p_string,uint8_t length,X_Boolean *isOK);


#ifdef __cplusplus
		}
#endif

#endif


