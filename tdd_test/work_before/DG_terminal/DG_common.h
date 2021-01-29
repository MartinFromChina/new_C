#ifndef __DG_common_H
#define __DG_common_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_terminal.h"
#include "DG_protocol.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"


#define USE_INSERT_DEBUG 1

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

#define MAX_DG_FRAME_LENGTH       (50)


#ifdef __cplusplus
		}
#endif


#endif


