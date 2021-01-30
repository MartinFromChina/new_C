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
 	 X_Boolean                   isStartPoint;  // not suit here ,but seem doesn't metter
	 X_Boolean					 isExpectAckHasCome;
	 X_Boolean					 isReSend;
	 uint8_t					 wait_times;
	 uint16_t					 wait_counter;
	 uint8_t					 command_start_terminal;
	 uint8_t					 wait_source;
	 uint8_t					 wait_type;
	 uint32_t					 start_wait_time;
	 uint8_t					 resend_buf[MAX_DG_FRAME_LENGTH];
 }s_wait_ack;

 typedef struct
 {
 	uint8_t temperature_threshold;
	uint16_t DG_wave_speed;
 }s_terminal_inf;

typedef struct
{
	uint8_t terminal_num;
	uint8_t forward_num;
	uint8_t backward_num;
	const s_ProtocolAction *p_action;
	s_wait_ack *p_wait_ack;
	s_terminal_inf *p_info;
}s_terminal;

X_DATA_UNIT 			DG_unit_receive(X_Void);
X_Boolean 				DG_find_header(X_DATA_UNIT current_data);
e_find_other_process 	DG_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf);

#define APP_DG_TERMINAL_DEFINE(p_terminal,cur,forward,backward)   \
		PROTOCOL_RECV_DATA_BUF_DEF(CONCAT_2(p_terminal,_action),40,5,DG_unit_receive,DG_find_header,DG_find_others);\
		static const s_ProtocolAction CONCAT_2(p_terminal,_action_entry) = {		\
			CONCAT_2(p_terminal,_action),											\
			ProtocolRecvInit,														\
			ProtocolRecvProcess,													\
			ProtocolRecvGetFrame,													\
			DoesProtocolRecvInitOK,													\
		};																	\
		static s_wait_ack CONCAT_2(p_terminal,_wait_ack_entry) ={					\
			X_False,X_True,X_True,0,												\
			0,0,0,0,0,														\
			{0},															\
		};															\
		static s_terminal_inf CONCAT_2(p_terminal,_dg_info) ={		\
		0,0,														\
		};													\
		static const s_terminal CONCAT_2(p_terminal,_entry) = {	\
			cur,												\
			forward,											\
			backward,											\
			&CONCAT_2(p_terminal,_action_entry),				\
			&CONCAT_2(p_terminal,_wait_ack_entry),					\
			&CONCAT_2(p_terminal,_dg_info),						\
		}; 															 \
		static const s_terminal *  p_terminal =  &CONCAT_2(p_terminal,_entry)

typedef X_Boolean (*func_send)(uint8_t node_num,uint32_t sent_time,uint8_t *p_buf,uint8_t length);
X_Void DG_TerminalInit(func_send p_src);

X_Void MainLoopHandle(const s_terminal * p_terminal,uint32_t current_time);
X_Void UartIrqHandle(const s_terminal * p_terminal,uint8_t data);


#ifdef __cplusplus
		}
#endif


#endif


