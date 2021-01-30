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

typedef struct 
{
	X_Boolean isRecv;
	uint8_t current_node_num;
	uint8_t data[50];
	uint16_t length;
	uint32_t time;
}s_monitor_table;

typedef X_Void (*p_data_monitor)(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);

X_Void TestCommonInit(p_data_monitor p_m);
X_Void TestCommonDeInit(X_Void);
X_Void DisableLogDebug(X_Void); // called it after TestCommonInit
X_Void SetTemporaryDistance(uint16_t distance);
X_Void mockable_Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
X_Void mockable_Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
uint16_t mockable_GetDistance(X_Void);

#ifdef __cplusplus
		}
#endif


#endif


