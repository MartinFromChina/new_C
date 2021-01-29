#ifndef __DG_PROTOCOL_H
#define __DG_PROTOCOL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "DG_terminal.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"


#define USE_INSERT_DEBUG 1

#if (USE_INSERT_DEBUG != 0)
	#define INSERT(log_method)  insert_##log_method
#else
	#define INSERT(log_method)  remove_##log_method
#endif

#define BASIC_INFO_SET_TYPE 		0xf0
#define BASIC_INFO_GET_TYPE 		0xf1
#define RTC_CONFIG_TYPE 			0x52
#define DG_INFO_MUL_GET_TYPE 		0x55
#define DG_TERMINAL_LOST_TYPE 		0x88
#define IMMEDIATELY_ACK_TYPE        0xfe


#pragma pack(1)
typedef struct
{
	uint16_t header; // 0x55aa
	uint8_t  length;
	uint8_t  src;
	uint8_t  dest;
	uint8_t  type;
}s_DG_data_common;

typedef struct
{
	s_DG_data_common common;
	uint8_t  terminal;
	uint8_t  double_forward_terminal;
	uint8_t  forward_terminal;
	uint8_t  backward_terminal;
	uint8_t  double_backward_terminal;
	uint8_t   reserve;
	uint8_t   check_sum;
}s_DG_basic_info_set;

typedef struct
{
	s_DG_data_common common;
	uint8_t   terminal;
	uint8_t   check_sum;
}s_DG_basic_info_get;

typedef struct
{
	s_DG_data_common common;
	uint8_t   start_terminal;
	uint16_t  year;
	uint8_t   mounth;
	uint8_t   day;
	uint8_t   hour;
	uint8_t   minute;
	uint8_t   second;
	uint8_t   check_sum;
}s_DG_RTC_config;

typedef struct
{
	s_DG_data_common common;
	uint8_t   start_terminal;
	uint8_t   reserve;
	uint8_t   check_sum;
}s_DG_info_mul_get;

typedef struct
{
	s_DG_data_common common;
	uint8_t   lost_terminal;
	uint8_t   check_sum;
}s_DG_terminal_lost;


typedef struct
{
	uint16_t header;// 0x66cc
	uint16_t length;
	uint8_t  src;
	uint8_t  dest;
	uint8_t  type;
	uint8_t  local_terminal;
}s_DG_response_common;

typedef struct
{
	uint16_t header;// 0x66cc
	uint16_t length;//10
	uint8_t  src;
	uint8_t  dest;
	uint8_t  type;//0xfe
	uint8_t  data_or_command_type; 
	uint8_t checksum;
}s_DG_immedicate_ack;


#pragma pack()

typedef enum
{
	efm_for_me,
	efm_pass_me_up,
	efm_pass_me_down,
	efm_not_my_business,
}e_frame_and_me;


uint8_t GetType(uint8_t *p_buf);
uint16_t GetLength(uint8_t *p_buf);
uint8_t GetSrc(uint8_t *p_buf);
uint8_t GetDest(uint8_t *p_buf);
X_Boolean CopyFrame(uint8_t *p_src,uint8_t *p_dest);
X_Boolean SetSrcDest(uint8_t *p_buf,uint8_t src,uint8_t dest);
X_Boolean LoadCheckSum(uint8_t *p_buf,uint8_t length);
X_Boolean DoesItAboutMe(uint8_t me,uint8_t src,uint8_t dest);


#ifdef __cplusplus
		}
#endif


#endif


