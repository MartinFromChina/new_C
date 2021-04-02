#ifndef __LOOP_QUEUES_H
#define __LOOP_QUEUES_H
//
/****************************************************
 * the loop queue can't be interrupt by an irq !!!
 */


#ifdef __cplusplus
 extern "C"{
#endif
/*
#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
*/
#include "../x_cross_platform.h"
#include "../CommonMarco.h"

typedef enum
{
	SQO_Push,
	SQO_Pop,
	SQO_Clear,
}eSimpleQueueOperation;

typedef struct
{
	X_Boolean   isInit;
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

#define INVALID_LOOP_QUEUE_NODE_NUM  (0xFFFF)

#define APP_LOOPQUEUE_DEF_WITHOUT_POINTER(p_manager,max_node_number)            					\
		static uint8_t  CONCAT_2(p_manager,_loopqueue_node_buf)[max_node_number];	 				\
		static sListManagerParam CONCAT_2(p_manager,_loopqueue_param) = {X_False,0,0,0,0};    				\
		static const  sListManager  CONCAT_2(p_manager,_loopqueue_entry) = {						\
			CONCAT_2(p_manager,_loopqueue_node_buf),												\
			max_node_number,																		\
			&CONCAT_2(p_manager,_loopqueue_param),													\
		}

#define APP_LOOPQUEUE_DEF(p_manager,max_node_number)            									\
		static uint8_t  CONCAT_2(p_manager,_loopqueue_node_buf)[max_node_number];	 				\
		static sListManagerParam CONCAT_2(p_manager,_loopqueue_param) = {X_False,0,0,0,0};    				\
		static const  sListManager  CONCAT_2(p_manager,_loopqueue_entry) = {						\
			CONCAT_2(p_manager,_loopqueue_node_buf),												\
						max_node_number,														    \
			&CONCAT_2(p_manager,_loopqueue_param),													\
		};																						    \
		static const  sListManager * p_manager = &CONCAT_2(p_manager,_loopqueue_entry)

X_Void 		LoopQueueInitialize(const sListManager *p_manager);
uint16_t    LoopQueueFirstIn(const sListManager *p_manager,X_Boolean is_OccupyPermit);
uint16_t    LoopQueueFirstOut(const sListManager *p_manager);
X_Void      ClearLoopQueue(const sListManager *p_manager);
X_Void      RealseLoopQueueBuf(const sListManager *p_manager,uint8_t buf_num);
uint16_t    GetLoopQueueUsedNodeNumber(const sListManager *p_manager);
X_Boolean   DoesLoopQueueEmpty(const sListManager *p_manager);

/**********************
the param "X_Boolean is_OccupyPermit" is set to false when you want to protect your node ,
if the node is not permit occupy , other caller can't insert the node a new information by function SimpleQueueFirstIn, it suck here in a word
but still ClearSimpleQueue can clear it all

you could use SimpleQueueFirstOut to get the node information 
!!! even you called SimpleQueueFirstOut , the node is still yours ,not released to others
 when you finish using it ,you should call RealseSimpleQueueBuf to release the node 
*************************/

/**********************
if the node is permit occupy
just call SimpleQueueFirstIn,SimpleQueueFirstOut whenever you want 
*************************/

/**********************
if the return value is INVALID_NODE_NUM,
sth wrong ,call ClearSimpleQueue to reinit it
*************************/

typedef X_Void 		(*QueueInit)(const sListManager *p_manager);
typedef uint16_t    (*QueueFI)(const sListManager *p_manager,X_Boolean is_OccupyPermit);
typedef uint16_t    (*QueueFO)(const sListManager *p_manager);
typedef X_Void      (*QueueClear)(const sListManager *p_manager);
typedef X_Void      (*QueueRelease)(const sListManager *p_manager,uint8_t buf_num);
typedef uint16_t    (*QueueUsedNum)(const sListManager *p_manager);
typedef X_Boolean   (*QueueDoesEmpty)(const sListManager *p_manager);

typedef struct
 {
	uint16_t cur_push_node;
	uint16_t cur_pop_node;
	const sListManager 			*p_manager;
	const QueueInit 			queue_init;
	const QueueFI				queue_fi;
	const QueueFO				queue_fo;
	const QueueClear			queue_clear;
	const QueueRelease			queue_release;
	const QueueUsedNum			queue_num;
	const QueueDoesEmpty		queue_empty;
 }s_QueueOperation;


#ifdef __cplusplus
}
#endif

#endif
