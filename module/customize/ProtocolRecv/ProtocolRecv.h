#ifndef __PROTOCOLRECV_H
#define __PROTOCOLRECV_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"
#include "../../common/loopqueue/loop_queues.h"


#ifndef X_DATA_UNIT   
#define X_DATA_UNIT uint8_t
#endif

#define MAX_FRAME_LENGTH         50
#define MAX_FRAME_CHCHE_NUM      20   
#define BAD_FRAME_FLAG 			0x77
#define RECV_PROCESS_FLAG 		0x88  //
#define GOOD_FRAME_FLAG 		0x99  //


typedef enum{
	FOP_idle,
	FOP_successed,
	FOP_errordata,
	FOP_inprocess,
	FOP_nospace,
}e_find_other_process;

typedef X_Void 		(*QueueInit)(const sListManager *p_manager);
typedef uint16_t    (*QueueFI)(const sListManager *p_manager,X_Boolean is_OccupyPermit);
typedef uint16_t    (*QueueFO)(const sListManager *p_manager);
typedef X_Void      (*QueueClear)(const sListManager *p_manager);
typedef X_Void      (*QueueRelease)(const sListManager *p_manager,uint8_t buf_num);
typedef uint16_t    (*QueueUsedNum)(const sListManager *p_manager);
typedef X_Boolean   (*QueueDoesEmpty)(const sListManager *p_manager);


typedef X_DATA_UNIT (*unit_receive)(X_Void);
typedef X_Boolean (*find_header)(X_DATA_UNIT current_data);
typedef e_find_other_process (*find_others_and_checksum)(X_DATA_UNIT current_data,e_find_other_process const*p_fop,X_DATA_UNIT *p_buf);

typedef struct
{
	X_Boolean isHeaderFind;
	e_find_other_process process;
}s_ProtocolRecvProcess;
 typedef struct
 {
	uint16_t cur_push_node;
	uint16_t cur_pop_node;
	const sListManager *p_manager;
	QueueInit 			queue_init;
	QueueFI				queue_fi;
	QueueFO				queue_fo;
	QueueClear			queue_clear;
	QueueRelease		queue_release;
	QueueUsedNum		queue_num;
	QueueDoesEmpty		queue_empty;
 }s_QueueOperation;

 typedef struct
{
    X_DATA_UNIT 	*  const		p_buf;
	X_Boolean 						*isInitOK;
	const uint16_t              	max_frame_length;
	const uint16_t              	max_frame_num;
    unit_receive 					func_recv;
	find_header 					func_fheader;
	find_others_and_checksum 		func_fothers;
	s_ProtocolRecvProcess *const	p_process;
    s_QueueOperation      *const    p_operation;   
}sProtocolRecv;

#define PROTOCOL_RECV_DATA_BUF_DEF(p_manager,max_frame_length,max_faram_to_cache,recv,fheader,fothers)  \
			static X_DATA_UNIT  CONCAT_2(p_manager,_recv_buf)[max_faram_to_cache][max_frame_length + 1];     \
			static X_Boolean    CONCAT_2(p_manager,_isInitOk) = X_False;  									\
			static s_ProtocolRecvProcess CONCAT_2(p_manager,_Protocol_recv_process) = {X_False,FOP_idle};  \
			SIMPLE_LOOPQUEUE_DEF(CONCAT_2(p_manager,_queue),max_faram_to_cache);							\
			static s_QueueOperation      CONCAT_2(p_manager,_Protocol_recv_queue) = {						\
					0,																				\
					0,																				\
					CONCAT_2(p_manager,_queue),														\
					SimpleQueueInitialize,															\
					SimpleQueueFirstIn,																\
					SimpleQueueFirstOut,															\
					ClearSimpleQueue,																\
					RealseSimpleQueueBuf,															\
					GetSimpleQueueUsedNodeNumber,													\
					DoesSimpleQueueEmpty,															\
			};																						\
			static const sProtocolRecv CONCAT_2(p_manager,_protocol_recv) = {							\
					&CONCAT_2(p_manager,_recv_buf)[0][0],												\
					&CONCAT_2(p_manager,_isInitOk),														\
					max_frame_length,																	\
					max_faram_to_cache,																	\
					recv,																				\
					fheader,																			\
					fothers,																			\
					& CONCAT_2(p_manager,_Protocol_recv_process),										\
					&CONCAT_2(p_manager,_Protocol_recv_queue),											\
			};																							\
			static const sProtocolRecv *p_manager = &CONCAT_2(p_manager,_protocol_recv)


X_Boolean ProtocolRecvInit(const sProtocolRecv *p_manager);
X_Void ProtocolRecvProcess(const sProtocolRecv *p_manager);// call it in a recv_irq or main_loop
X_Boolean ProtocolRecvGetFrame(const sProtocolRecv *p_manager,X_DATA_UNIT **pp_buf);
X_Boolean DoesProtocolRecvInitOK(const sProtocolRecv *p_manager);

#ifdef __cplusplus
		}
#endif

#endif


