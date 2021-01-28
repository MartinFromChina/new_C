#ifndef __TEST_COMMON_H
#define __TEST_COMMON_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../../module/common/x_cross_platform.h"
#if (USE_TDD_MOCK != 0)
	#define MOCKABLE(method)  mockable_##method
#else
	#define MOCKABLE(method)  method
#endif

X_Void mockable_Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
X_Void mockable_Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);

#ifdef __cplusplus
		}
#endif


#endif


