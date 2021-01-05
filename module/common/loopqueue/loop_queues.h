#ifndef __LOOP_QUEUES_H
#define __LOOP_QUEUES_H
//
/****************************************************
 * the loop queue can't be interrupt by an irq !!!
 */


#ifdef __cplusplus
 extern "C"{
#endif

#include "..\KeilMDK.h"
#include "..\CommonMarco.h"

typedef enum
{
	SQO_Push,
	SQO_Pop,
	SQO_Clear,
}eSimpleQueueOperation;

typedef struct
{
	uint8_t  state;
	uint16_t first_in_node_num;
	uint16_t first_out_node_num;
	uint16_t used_node_num;
}sListManagerParam;

typedef struct
{
	uint8_t     *p_buf;
	uint16_t  	ValidNodeNumber;
	sListManagerParam *p_LMP;
}sListManager;

#define SIMPLE_LOOPQUEUE_DEF_WITHOUT_POINTER(p_manager,max_node_number)            					\
		static uint8_t  CONCAT_2(p_manager,_loopqueue_node_buf)[max_node_number];	 				\
		static sListManagerParam CONCAT_2(p_manager,_loopqueue_param) = {0,0,0,0};    				\
		static const  sListManager  CONCAT_2(p_manager,_loopqueue_entry) = {						\
			CONCAT_2(p_manager,_loopqueue_node_buf),												\
			max_node_number,																		\
			&CONCAT_2(p_manager,_loopqueue_param),													\
		}

#define SIMPLE_LOOPQUEUE_DEF(p_manager,max_node_number)            									\
		static uint8_t  CONCAT_2(p_manager,_loopqueue_node_buf)[max_node_number];	 				\
		static sListManagerParam CONCAT_2(p_manager,_loopqueue_param) = {0,0,0,0};    				\
		static const  sListManager  CONCAT_2(p_manager,_loopqueue_entry) = {						\
			CONCAT_2(p_manager,_loopqueue_node_buf),												\
			{			max_node_number,																		\
			&CONCAT_2(p_manager,_loopqueue_param),													\
		} ;																						    \
		static const  sListManager * p_manager = &CONCAT_2(p_manager,_loopqueue_entry)

X_Void 		SimpleQueueInitialize(const sListManager *p_manager);
uint16_t    SimpleQueueFirstIn(const sListManager *p_manager,X_Boolean *isOK,X_Boolean is_OccupyPermit);
uint16_t    SimpleQueueFirstOut(const sListManager *p_manager,X_Boolean *isOK);
X_Void      ClearSimpleQueue(const sListManager *p_manager);
X_Void      RealseSimpleQueueBuf(const sListManager *p_manager,uint8_t buf_num);
uint16_t    GetSimpleQueueUsedNodeNumber(const sListManager *p_manager);
X_Boolean   DoesSimpleQueueEmpty(const sListManager *p_manager);
#ifdef __cplusplus
}
#endif

#endif
