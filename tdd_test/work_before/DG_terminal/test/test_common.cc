#include "test_common.h"
#include "../../../google_test/include/gtest/include/gtest/gtest.h"
#include "../../../google_test/include/include/gmock/gmock.h"
#include "../DG_protocol.h"

#define  RECV_DEBUG   1

static uint8_t recv_node_backup = 0xff,send_node_backup = 0xff;
X_Void mockable_Recv_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{
	uint8_t i;
	if(recv_node_backup != current_node_num)
	{
		recv_node_backup = current_node_num;
		INSERT(LogDebug)(RECV_DEBUG,("------------------------------------------------ \r\n"));
	}
	INSERT(LogDebug)(RECV_DEBUG,(">  >  terminal %d receive data ",current_node_num));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(RECV_DEBUG,("%2x,",p_data[i]));
	}
	INSERT(LogDebug)(RECV_DEBUG,("      at time %d \r\n",time));
}

X_Void mockable_Send_Monitor(uint8_t current_node_num,uint8_t *p_data,uint16_t length,uint32_t time)
{
	uint8_t i;
	if(send_node_backup != current_node_num)
	{
		send_node_backup = current_node_num;
		INSERT(LogDebug)(RECV_DEBUG,("------------------------------------------------ \r\n"));
	}
	INSERT(LogDebug)(RECV_DEBUG,("<  <  terminal %d send data ",current_node_num));
	for(i=0;i<length;i++)
	{
		INSERT(LogDebug)(RECV_DEBUG,("%2x,",p_data[i]));
	}
	INSERT(LogDebug)(RECV_DEBUG,("      at time %d \r\n",time));
}


