#ifndef __NODE_INTERCONNECT_H
#define __NODE_INTERCONNECT_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"

#define INVALID_NODE_NUM (0xFF)
#define INVALID_NODE_DISTANCE (0xFFFF)

#define MAX_WAVE_FRAME_LENGTH  50
#define MAX_WAVE_ONE_NODE_HOLD  10



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

typedef enum
{
	NF_idle,
	NF_inter_node,
	NF_end_node,
}e_node_flag;


typedef struct
{
	X_Boolean isDisapper;
	e_direction direct;
	uint16_t max_trans_distance;// speed is 1 unit trans_distance/s, so after max_trans_distance's seconds , the wave is disapper
	uint8_t passed_node_cnt;
	uint8_t content_length;
	uint8_t context[MAX_WAVE_FRAME_LENGTH];
}s_wave;

typedef struct _s_node
{
 	uint8_t node_number;
	uint8_t forware_node;
	uint16_t  forware_distance;
	uint8_t backward_node;
	uint16_t  backward_distance;
	uint8_t current_hold_wave;
	s_wave  wave_trans[MAX_WAVE_ONE_NODE_HOLD];
}s_node;

typedef struct _s_node_manager
{
	e_node_flag 		flag;
	s_node 				*p_node;
	X_Boolean (*handle)(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length);
	_s_node_manager 	*p_previous;
	_s_node_manager 	*p_next;
}s_node_manager;

typedef X_Boolean (*p_node_handle)(_s_node_manager *p_manager,uint8_t current_node_num,uint8_t *p_data,uint16_t length);

s_node_manager * 	WaveTransInit(p_node_handle handle);
X_Void 				WaveTransDeInit(X_Void);
X_Boolean 			RunNodeCommunicationProcess(X_Void);
X_Boolean 			NodeAdd(s_node_manager *p_manager,s_node_manager *p_new_node);
uint16_t 			GetNodeNum(X_Void);
uint16_t 			GetDistanceBetweenNode(uint8_t node_num1,uint8_t node_num2);
uint32_t 			GetTime(X_Void);
X_Boolean 			SendWave(s_node_manager *p_manager,uint32_t sys_time,uint8_t node_num,s_wave *p_wave);


#ifdef __cplusplus
		}
#endif


#endif

