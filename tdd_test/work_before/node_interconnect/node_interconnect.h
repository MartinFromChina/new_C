#ifndef __NODE_INTERCONNECT_H
#define __NODE_INTERCONNECT_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"

#define INVALID_NODE_NUM (0xFF)

#define USE_INSERT_DEBUG 1

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

typedef struct
{
	uint8_t wave_num;
}s_node_handler;

typedef X_Boolean (*p_node_handle)(s_node_handler message);


typedef struct
{
 	uint8_t node_number;
	char 	node_name[50];
	uint8_t forware_node;
	uint8_t backward_node;
	s_node_handler node_message;
	uint8_t node_message_num;
	X_Boolean (*node_handle)(s_node_handler message);
}s_node;

typedef enum
{
	NF_idle,
	NF_inter_node,
	NF_end_node,

}e_node_flag;


typedef struct _s_node_manager
{
	e_node_flag 		flag;
	s_node 				*p_node;
	_s_node_manager 	*p_next;
}s_node_manager;


X_Boolean RunNodeCommunicationProcess(X_Void);
s_node_manager * WaveTransInit(X_Void);
X_Boolean NodeAdd(s_node_manager *p_manager,s_node_manager *p_new_node);
uint16_t GetNodeNum(X_Void);

#ifdef __cplusplus
		}
#endif


#endif

