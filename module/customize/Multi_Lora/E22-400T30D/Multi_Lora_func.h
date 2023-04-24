#ifndef __LORA_FUNC_H
#define __LORA_FUNC_H

#ifdef __cplusplus
	extern "C"{
#endif

#include "LogDebugSwitch.h"
////////#include <string.h>

#define USE_RSSI_BYTE                 0     // 		据某位技术支持说开启这个会增加延迟
		
#define MAX_LORA_SEND_DATA_LENGTH     240 // 	想要达到缓存容量的400，需要手动分包加延时，索性不用这个鸡肋的缓存功能了
#define MAX_LORA_RECV_DATA_LENGTH     240

#define LORA_READ_CMD                 0xc1
typedef enum
{
	LM_Trans = 0,
	LM_Wor,
	LM_Config,
	LM_DeepSleep,
}eLoraMode;

typedef enum
{
	LP_InProcess,
	LP_Finished,
	LP_Error,
}eLoraProcess;

typedef enum
{
	LE_Successed,
	LE_Failed,
	LE_Check,
	LE_TimeOut,
}eLoraEvent;

typedef X_Boolean (*set_work_mode)(uint8_t mode);
typedef X_Boolean (*get_byte)(uint8_t *p_data);
typedef X_Boolean (*send_buf)(const uint8_t *p_buf,uint16_t length);
typedef X_Void    (*clear_recv_cache)(X_Void);
typedef uint32_t (*get_time)(X_Void);
typedef X_Boolean (*set_time)(uint32_t time_ms);
typedef X_Boolean (*event_handler)(eLoraEvent evt,uint8_t *p_data,uint16_t length);
typedef X_Boolean (*does_aux_busy)(X_Void);

/*
there may be some dummy data trigger the lora get data port when the lora haradware just power up 
the func "clear_recv_cache" could clear those error data  so that the module has stable performence.
*/
X_Boolean mFunc_LoraInit(set_work_mode p_func_set,
											get_byte p_func_get_byte,send_buf p_func_send_buf,clear_recv_cache p_func_clear_recv_cache,
											get_time p_func_get_time,set_time p_func_set_time);
/********************************lora config ***********************************************/
#define MAX_LORA_CONFIG_DATA_LENGTH          12 // header + reg_value : 3 + 9 = 12;

typedef enum
{
	Cb_8N1 = 0,
	Cb_801,
	Cb_8E1,
}eCheckBits;

typedef enum
{
	AS_0_3k = 0,
	AS_1_2k,
	AS_2_4k ,
	AS_4_8k,
	AS_9_6k,
	AS_19_2k,
	AS_38_4k,
	AS_62_5k,
}eAirSpeed;

typedef enum
{
	TP_30dbm = 0,
	TP_27dbm,
	TP_24dbm,
	TP_21dbm,
}eTransPower;

typedef enum
{
	TM_transparent = 0,
	TM_p2p,
}eTransMode;

typedef struct
{
	uint16_t       module_addr;
	uint8_t        net_id;
	uint32_t       baud_rate;
	eCheckBits     check_bits;
	eAirSpeed      air_speed;
	uint16_t       subpackage_length;
	X_Boolean      isRssiEnable;
	eTransPower    trans_power;
	uint8_t        channel;
	X_Boolean      isRssiByteEnable;
	eTransMode     trans_mode;
	X_Boolean      isLBT_Enable; // listen before talk;
	uint16_t       WOR_period_ms;
	uint16_t       secret_key; 
}sLoraConfig;

#define LORA_DEFAULT_ADDR                (0x1234)
#define LORA_DEFAULT_NET_ID              0 
#define LORA_DEFAULT_BAUD_RATE           9600
#define LORA_DEFAULT_CHECK_BITES         Cb_8N1
#define LORA_DEFAULT_AIR_SPEED           AS_38_4k
#define LORA_DEFAULT_SUBPACKAGE_LENGTH   240
#define LORA_DEFAULT_RSSI_FUNCTION       X_False
#define LORA_DEFAULT_TRANS_POWER         TP_30dbm
#define LORA_DEFAULT_CHANNEL             23
#define LORA_DEFAULT_RSSI_BYTE_REPORT    (USE_RSSI_BYTE != 0)
#define LORA_DEFAULT_TRANS_MODE          TM_p2p
#define LORA_DEFAULT_LBT_FUNCTION        X_False
#define LORA_DEFAULT_WOR_PERIOD          500
#define LORA_DEFAULT_SECRET_KEY          (0x0000)

#define LORA_CONFIG_DEFAULT_PARAM		{	LORA_DEFAULT_ADDR, \
											LORA_DEFAULT_NET_ID, \
											LORA_DEFAULT_BAUD_RATE, \
											LORA_DEFAULT_CHECK_BITES, \
											LORA_DEFAULT_AIR_SPEED, \
											LORA_DEFAULT_SUBPACKAGE_LENGTH, \
											LORA_DEFAULT_RSSI_FUNCTION, \
											LORA_DEFAULT_TRANS_POWER, \
											LORA_DEFAULT_CHANNEL, \
											LORA_DEFAULT_RSSI_BYTE_REPORT, \
											LORA_DEFAULT_TRANS_MODE, \
											LORA_DEFAULT_LBT_FUNCTION, \
											LORA_DEFAULT_WOR_PERIOD, \
											LORA_DEFAULT_SECRET_KEY	} 
//{}里面每一个宏两个空格后的 \ 必须要有，不然会被识别为里面的第一个宏
/********************************************************************************/
X_Void mFunc_LoraConfigReset(sLoraConfig * p_config,uint8_t retry_times,event_handler handler);
eLoraProcess mFunc_LoraConfigHandle(X_Void);
X_Boolean mFunc_LoraGetConfigCMD(uint8_t *p_data ,uint8_t *p_length);

X_Void mFunc_LoraAuxMonitorReg(does_aux_busy aux);

typedef X_Boolean (*p_lora_send_rec_cb)(eLoraEvent event,uint8_t *p_data,uint16_t length);

/******************************************************************
X_Boolean send_recv_cb_example(eLoraEvent event,uint8_t *p_data,uint16_t length)
{
	X_Boolean isOK = X_False;
	switch(event)
	{
		case LE_Successed:
			isOK = X_True;
			break;
		case LE_TimeOut:
			break;
		case LE_Failed:
			break;
		case LE_Check:
			break;
		default:
			break;
	}
	return isOK;
}
*********************************************************************/
X_Void mFunc_Lora_ReinitFuncRegister(X_Void (*reset)(X_Void));
X_Void mFunc_Lora_SendRecvReset(X_Void);
X_Void mFunc_Lora_SendRecvHandle(X_Void);

X_Void mFunc_Lora_SendAgain(X_Void);
X_Void mFunc_Lora_CurRecvProcessTerminate(X_Void);
X_Void mFunc_Lora_EnableContinueReceive(X_Void);
X_Void mFunc_Lora_DisableContinueReceive(X_Void);
X_Boolean mFunc_Lora_SendRequest(const uint8_t *p_data,uint16_t length,p_lora_send_rec_cb p_send_cb
											,uint16_t rec_time_in_ms,p_lora_send_rec_cb p_recv_cb);

X_Boolean mFunc_Lora_DoesNewDataCome(uint8_t **pp_data,uint16_t *p_length);
X_Boolean mFunc_Does_LoraModuleBusy(X_Void);

#ifdef __cplusplus
		}
#endif

#endif
