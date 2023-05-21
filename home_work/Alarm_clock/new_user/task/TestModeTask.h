#ifndef __TESTMODETASK_H
#define __TESTMODETASK_H

#ifdef __cplusplus
	extern "C"{
#endif

#if (USE_TDD_MOCK != 0)
	#include "../../../test/tdd_test_common.h"
#else
	#include "LogDebugSwitch.h"
#endif

X_Void testmode_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);      

#ifdef __cplusplus
		}
#endif

#endif
