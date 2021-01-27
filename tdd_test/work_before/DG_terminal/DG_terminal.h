#ifndef __DG_TREMINAL_H
#define __DG_TREMINAL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "../../../module/common/CommonMarco.h"
#include "DG_common.h"

typedef struct
{
	uint8_t terminal_num;
	uint8_t forward_num;
	uint8_t backward_num;
}s_terminal;

#define APP_DG_TERMINAL_DEFINE(p_terminal,cur,forward,backward)   \
		static const s_terminal CONCAT_2(p_terminal,_entry) = {	\
			cur,												\
			forward,											\
			backward,											\
		}; 															 \
		static const s_terminal *  p_terminal =  &CONCAT_2(p_terminal,_entry)


X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time);
X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


