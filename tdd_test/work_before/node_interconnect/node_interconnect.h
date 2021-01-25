#ifndef __NODE_INTERCONNECT_H
#define __NODE_INTERCONNECT_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"

#define INVALID_NODE_NUM (0xFF)
#define MAX_WAVE_FRAME_LENGTH  50


#define USE_INSERT_DEBUG 1

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

typedef enum
{
	ED_forward,
	ED_backward,
	ED_bidirection,
}e_direction;

typedef struct
{
	e_direction direct;
	uint16_t max_trans_distance;// speed is 1 unit trans_distance/s, so after max_trans_distance's seconds , the wave is disapper
	uint8_t content_length;
	uint8_t context[MAX_WAVE_FRAME_LENGTH];
}s_wave;

typedef struct
{
	uint8_t wave_num;
}s_node_handler;


typedef struct _s_node
{
 	uint8_t node_number;
	uint8_t forware_node;
	uint8_t backward_node;
	s_node_handler node_info;
	uint8_t node_message_num;
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
	X_Boolean (*handle)(_s_node_manager *p_manager,s_node *p_cur_node);
	_s_node_manager 	*p_next;
}s_node_manager;

typedef X_Boolean (*p_node_handle)(_s_node_manager *p_manager,s_node *p_cur_node);

X_Boolean RunNodeCommunicationProcess(X_Void);
s_node_manager * WaveTransInit(p_node_handle handle);
X_Void WaveTransDeInit(X_Void);

X_Boolean NodeAdd(s_node_manager *p_manager,s_node_manager *p_new_node);
uint16_t GetNodeNum(X_Void);
uint32_t GetTime(X_Void);
X_Boolean SendWave(s_node_manager *p_manager,uint32_t sys_time,uint8_t node_num,s_wave *p_wave);


#ifdef __cplusplus
		}
#endif


#endif

