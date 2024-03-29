#ifndef __DG_PROTOCOL_H
#define __DG_PROTOCOL_H


#ifdef __cplusplus
	extern "C"{
#endif

#include "../../../module/common/x_cross_platform.h"
#include "../../../module/common/InsertLog/InsertLogDebug.h"
#include "DG_terminal.h"


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
#define ERROR_REPORT_TYPE           0x88

#define MULTICAST_PREFIX            (0x50)
#define MULTICAST_MASK           	(0xf0)
#define MULTICAST_GET_INFO           0x55
#define MULTICAST_SET_SPEED          0x56


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
	uint8_t   reserve;  //0
	uint8_t   check_sum;
}s_DG_info_mul_get;

typedef struct
{
	s_DG_data_common common;
	uint8_t   start_terminal;
	uint16_t   speed;  
	uint8_t   check_sum;
}s_DG_speed_mul_set;



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

typedef struct
{
	uint16_t header;// 0x66cc
	uint16_t length;//10
	uint8_t  src;
	uint8_t  dest;
	uint8_t  type;//0xfe
	uint8_t  lost_terminal; 
	uint8_t checksum;
}s_ERR_report;

#pragma pack()

typedef enum
{
	efd_for_me,
	efd_trans_up,
	efd_trans_down,
}e_frame_direction;

typedef enum
{
	eft_data,
	eft_command,
}e_frame_type;


uint8_t GetType(uint8_t *p_buf);
uint16_t GetLength(uint8_t *p_buf);
uint8_t GetSrc(uint8_t *p_buf);
uint8_t GetDest(uint8_t *p_buf);
X_Boolean CopyFrame(uint8_t *p_src,uint8_t *p_dest);
X_Boolean SetSrcDest(uint8_t *p_buf,uint8_t src,uint8_t dest);
X_Boolean LoadCheckSum(uint8_t *p_buf,uint8_t length);
X_Boolean DoesItAboutMeAndComeFromAdjcent(uint8_t me,uint8_t foreward,uint8_t backward,uint8_t src,uint8_t dest);
X_Boolean DoesImediatelyAckForMe(uint8_t *p_buf,uint8_t me,uint8_t src,uint8_t expect_type);
X_Boolean DoesMultiCastType(uint8_t type);
X_Boolean DG_CommandHandle(const s_terminal *p_terminal,e_frame_type frame_type,e_frame_direction direct,uint8_t type,uint8_t *p_recv,uint8_t *p_send);


#ifdef __cplusplus
		}
#endif


#endif


