#ifndef __DG_TREMINAL_H
#define __DG_TREMINAL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "../../../module/common/CommonMarco.h"
#include "DG_common.h"
#include "../../../module/customize/ProtocolRecv/ProtocolRecv.h"

#define COMMON_WIRELESS_DIRECTION    ED_bidirection
#define COMMON_WIRELESS_DISTANCE     20

typedef struct
 {
 	const sProtocolRecv *p_manager;
	X_Boolean 	(*Init)(const sProtocolRecv *p_manager);
	X_Void 		(*Process)(const sProtocolRecv *p_manager);// call it in a recv_irq or main_loop
	X_Boolean 	(*GetFrame)(const sProtocolRecv *p_manager,X_DATA_UNIT **pp_buf);
	X_Boolean 	(*DoesInitOK)(const sProtocolRecv *p_manager);

 }s_ProtocolAction;

typedef struct
{
	uint8_t terminal_num;
	uint8_t forward_num;
	uint8_t backward_num;
	const s_ProtocolAction *p_action;
}s_terminal;

X_DATA_UNIT empty_unit_receive(X_Void);
X_Boolean empty_find_header(X_DATA_UNIT current_data);
e_find_other_process empty_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf);

#define APP_DG_TERMINAL_DEFINE(p_terminal,cur,forward,backward)   \
		PROTOCOL_RECV_DATA_BUF_DEF(CONCAT_2(p_terminal,_action),40,5,empty_unit_receive,empty_find_header,empty_find_others);\
		static const s_ProtocolAction CONCAT_2(p_terminal,_action_entry) = {		\
			CONCAT_2(p_terminal,_action),											\
			ProtocolRecvInit,														\
			ProtocolRecvProcess,													\
			ProtocolRecvGetFrame,													\
			DoesProtocolRecvInitOK,													\
		};																	\
		static const s_terminal CONCAT_2(p_terminal,_entry) = {	\
			cur,												\
			forward,											\
			backward,											\
			&CONCAT_2(p_terminal,_action_entry),				\
		}; 															 \
		static const s_terminal *  p_terminal =  &CONCAT_2(p_terminal,_entry)

typedef X_Boolean (*func_send)(uint8_t node_num,uint32_t sent_time,uint8_t *p_buf,uint8_t length);
X_Void SentFuncInit(func_send p_src);

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time);
X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


