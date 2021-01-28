#ifndef __TREMINAL2_H
#define __TREMINAL2_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_terminal.h"

X_Void Terminal2Init(X_Void);
X_Void main_loop_2(uint32_t current_time);
X_Void UartIrqCallBack_2(uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


