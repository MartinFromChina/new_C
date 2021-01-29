#include "test_common.h"
#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "../DG_protocol.h"

static uint8_t recv_node_backup = 0xff,send_node_backup = 0xff;
static p_data_monitor p_monitor = (p_data_monitor)0;
static uint8_t Recv_debug_Flag = 0;
static uint16_t temporary_distance = 0;



X_Void TestCommonInit(p_data_monitor p_m)
{
	p_monitor = p_m;
	recv_node_backup = 0xff;
	send_node_backup = 0xff;
	Recv_debug_Flag = 1;
	temporary_distance = 0;
}
X_Void TestCommonDeInit(X_Void)
{
	p_monitor = (p_data_monitor)0;
	recv_node_backup = 0xff;
	send_node_backup = 0xff;
	Recv_debug_Flag = 0;
	temporary_distance = 0;
}
X_Void DisableLogDebug(X_Void)
{
	Recv_debug_Flag = 0;
}
//X_Void EnableLogDebug(X_Void)
X_Void SetTemporaryDistance(uint16_t distance)
{
	temporary_distance = distance;
}
X_Void mockable_Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{
	uint8_t i;
	if(recv_node_backup != current_node_num)
	{
		recv_node_backup = current_node_num;
		INSERT(LogDebug)(Recv_debug_Flag,("------------------------------------------------ \r\n"));
	}
	INSERT(LogDebug)(Recv_debug_Flag,("---->  >  terminal %d receive data ",current_node_num));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(Recv_debug_Flag,("%2x,",p_data[i]));
	}
	INSERT(LogDebug)(Recv_debug_Flag,("      at time %d \r\n",time));
	if(p_monitor != X_Null) {p_monitor(X_True,current_node_num,p_data,length,time);}
}

X_Void mockable_Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{
	uint8_t i;
	if(send_node_backup != current_node_num)
	{
		send_node_backup = current_node_num;
		INSERT(LogDebug)(Recv_debug_Flag,("------------------------------------------------ \r\n"));
	}
	INSERT(LogDebug)(Recv_debug_Flag,("<  <---  terminal %d send data ",current_node_num));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(Recv_debug_Flag,("%2x,",p_data[i]));
	}
	INSERT(LogDebug)(Recv_debug_Flag,("      at time %d \r\n",time));
	if(p_monitor != X_Null) {p_monitor(X_False,current_node_num,p_data,length,time);}
}
uint16_t mockable_GetDistance(X_Void)
{
	return (temporary_distance != 0)? temporary_distance : COMMON_WIRELESS_DISTANCE;
}


