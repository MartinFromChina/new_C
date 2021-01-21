#ifndef __PROTOCOLRECV_H
#define __PROTOCOLRECV_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"

#ifndef X_DATA_UNIT   
#define X_DATA_UNIT uint8_t
#endif

#define MAX_FARME_LENGTH         50
#define MAX_FRAME_CHCHE_NUM      20   

typedef enum{
	FOP_successed,
	FOP_failed,
	FOP_inprocess,
}e_find_other_process;

typedef X_DATA_UNIT (*unit_receive)(X_Void);
typedef X_Boolean (*find_header)(X_DATA_UNIT current_data,X_DATA_UNIT *p_header_buf,uint16_t *p_header_length);
typedef e_find_other_process (*find_others_and_checksum)(X_DATA_UNIT current_data,X_DATA_UNIT *p_other_buf,uint16_t *p_other_unit);

 typedef struct
{
    X_DATA_UNIT 	*  const	p_buf;
	X_Boolean                   *isInitCalled;
	X_Boolean 					*isInitOK;
	const uint16_t              max_frame_length;
	const uint16_t              max_frame_num;
    unit_receive 				func_recv;
	find_header 				func_fheader;
	find_others_and_checksum 	func_fothers;
}sProtocolRecv;

#define PROTOCOL_RECV_DATA_BUF_DEF(p_manager,max_frame_length,max_faram_to_cache,recv,fheader,fothers)  \
			static X_DATA_UNIT  CONCAT_2(p_manager,_recv_buf)[max_faram_to_cache][max_frame_length];     \
			static X_Boolean    CONCAT_2(p_manager,_isInitOk) = X_False,CONCAT_2(p_manager,_isInitCalled) = X_False;  \
			static const sProtocolRecv CONCAT_2(p_manager,_protocol_recv) = {							\
					&CONCAT_2(p_manager,_recv_buf)[0][0],												\
					&CONCAT_2(p_manager,_isInitCalled),													\
					&CONCAT_2(p_manager,_isInitOk),														\
					max_frame_length,																	\
					max_faram_to_cache,																	\
					recv,																				\
					fheader,																			\
					fothers,																			\
			};																							\
			static const sProtocolRecv *p_manager = &CONCAT_2(p_manager,_protocol_recv)


X_Boolean ProtocolRecvInit(const sProtocolRecv *p_manager);
X_Void ProtocolRecvProcess(const sProtocolRecv *p_manager);
X_Boolean ProtocolRecvGetFrame(const sProtocolRecv *p_manager,X_DATA_UNIT *p_buf,uint16_t length);
X_Boolean DoesProtocolRecvInitOK(const sProtocolRecv *p_manager);

#ifdef __cplusplus
		}
#endif

#endif


