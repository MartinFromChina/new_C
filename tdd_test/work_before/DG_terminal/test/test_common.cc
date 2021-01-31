#include "test_common.h"
#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "../DG_protocol.h"

static uint8_t recv_node_backup = 0xff,send_node_backup = 0xff;
static p_data_monitor p_monitor = (p_data_monitor)0;
static uint8_t Recv_debug_Flag = 0;
static uint16_t temporary_distance = 0;
static uint8_t MonitorListSize = 0,MonitorListIndex = 0;




X_Void TestCommonInit(p_data_monitor p_m,p_monitor_list_init init)
{
	p_monitor = p_m;
	recv_node_backup = 0xff;
	send_node_backup = 0xff;
	Recv_debug_Flag = 1;
	temporary_distance = 0;
	if(init != X_Null) {init();}
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

X_Void MonitorListInit(s_monitor_list *p_monitor,uint8_t size)
{
	uint8_t i;
	if(size > MAX_MONITOR_LIST_SIZE) {return;}
	if(p_monitor != X_Null)
	{
		for(i=0;i<size;i++)
		{
			p_monitor[i].ignore_times = 0xffff;
		}
	}
	MonitorListSize = size;
	MonitorListIndex = 0;
}
X_Boolean MonitorListAdd(s_monitor_list *p_monitor,s_monitor_list *p_src,uint16_t ignore_times)
{
	if(p_monitor == X_Null ) {return X_False;}
	if(ignore_times == 0 && p_src == X_Null) {return X_False;}

	if(MonitorListIndex < MonitorListSize)
	{
		if(ignore_times > 0)
		{
			p_monitor[MonitorListIndex].ignore_times = ignore_times;
			MonitorListIndex ++;
			return X_True;
		}
		else
		{
			p_monitor[MonitorListIndex].ignore_times 			= 0;
			p_monitor[MonitorListIndex].table.current_node_num 	= p_src ->table.current_node_num;
			p_monitor[MonitorListIndex].table.isRecv			= p_src ->table.isRecv;
			p_monitor[MonitorListIndex].table.length			= p_src ->table.length;
			p_monitor[MonitorListIndex].table.time				= p_src ->table.time;
			for(uint8_t i = 0 ;i<p_src ->table.length;i++)
			{
				p_monitor[MonitorListIndex].table.data[i] = p_src ->table.data[i];
			}
			MonitorListIndex ++;
			return X_True;
		}
	}
	return X_False;
}

