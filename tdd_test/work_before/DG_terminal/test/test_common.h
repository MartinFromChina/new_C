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

typedef struct 
{
	uint16_t ignore_times;
	s_monitor_table table;
}s_monitor_list;

#define MAX_MONITOR_LIST_SIZE   30

typedef X_Void (*p_data_monitor)(X_Boolean isRecv,uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
typedef X_Void (*p_monitor_list_init)(X_Void);

X_Void TestCommonInit(p_data_monitor p_m,p_monitor_list_init init);
X_Void TestCommonDeInit(X_Void);
X_Void DisableLogDebug(X_Void); // called it after TestCommonInit
X_Void SetTemporaryDistance(uint16_t distance);
X_Void mockable_Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
X_Void mockable_Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time);
uint16_t mockable_GetDistance(X_Void);
X_Void MonitorListInit(s_monitor_list *p_monitor,uint8_t size);
X_Boolean MonitorListAdd(s_monitor_list *p_monitor,s_monitor_list *p_src,uint16_t ignore_times);

#ifdef __cplusplus
		}
#endif


#endif


