#ifndef __LORATASK_H
#define __LORATASK_H

#ifdef __cplusplus
	extern "C"{
#endif

#if (USE_TDD_MOCK != 0)
	#include "../../../test/tdd_test_common.h"
#else
	#include "LogDebugSwitch.h"
#endif

X_Void lora_task(uint32_t x_task_param_ms,uint16_t task_id,uint8_t *p_isInitDone);
X_Boolean DoesTestStartCmdCome(uint8_t **pp_data);
X_Boolean DoesRelayLoraConfigDone(X_Void);
X_Void Insert_4GData_ToRelayLora(uint8_t *p_data_from_4G,uint16_t length_from_4G);
uint32_t GetBackupSrcAddr(X_Void);
X_Void Lora_SR_TestWithRssi(X_Void);
        
#ifdef __cplusplus
		}
#endif

#endif
