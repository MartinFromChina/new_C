#ifndef __TREMINAL1_H
#define __TREMINAL1_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_terminal.h"

X_Void Terminal1Init(X_Boolean isStartPoint);
X_Void main_loop_1(uint32_t current_time);
X_Void UartIrqCallBack_1(uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


