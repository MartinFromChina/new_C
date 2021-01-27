#ifndef __TREMINAL3_H
#define __TREMINAL3_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_terminal.h"

X_Void main_loop_3(uint32_t current_time);
X_Void UartIrqCallBack_3(uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


