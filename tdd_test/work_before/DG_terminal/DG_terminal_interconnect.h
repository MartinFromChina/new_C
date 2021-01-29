#ifndef __DG_TREMINAL_INTERCONNECT_H
#define __DG_TREMINAL_INTERCONNECT_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "../../../module/common/CommonMarco.h"
#include "DG_common.h"

X_Boolean TerminalInterconnectHandle(const s_terminal * p_terminal,uint8_t *p_recv,uint8_t *p_send);
#ifdef __cplusplus
		}
#endif


#endif


