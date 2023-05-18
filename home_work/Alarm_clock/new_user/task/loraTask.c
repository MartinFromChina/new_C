#include "loraTask.h"
#include "const_loop_scheduler.h"

#include "Multi_Lora.h"
#include "mFunc_SoftTimer.h"

#include "LoraCommuProtocol.h"
#include "../../../../../../../../module/common/AppCommon.h"
#include "DelayExtTask.h"
#include "mApp_MainTask.h"

#include "mDri_VGUS_LCD.h"
extern uint16_t send_ok_cnt,recv_ok_cnt;

static uint8_t dummy_buf[10] = {1,2,3,4,5,6,7,8,9,10};

extern X_Boolean isSingleTestOnce;
extern uint32_t TerminalNumberGet(X_Void);

static uint8_t relative_lora_channel = 20;
static uint32_t src_backup,dest_backup;
static p_lora_send p_lora_send_func = (p_lora_send)0;
static X_Void SR_Test_Receiver_LoadRssi(uint16_t env_rssi,uint16_t recv_rssi)
{
    uint8_t temp_buf[LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE + 3];
    uint16_t check_sum;
    temp_buf[0] = 0xab;
    temp_buf[1] = 0xcd;
    temp_buf[2] = relative_lora_channel;
    
    sLoraDataMultiParse * p_sr_test_response = (sLoraDataMultiParse *)&temp_buf[3];
    
    p_sr_test_response ->head.head    = LORA_RESP_FRAME_HEAD;
    p_sr_test_response ->head.src_addr  = FourBytesReverse(dest_backup);
    p_sr_test_response ->head.dest_addr    = FourBytesReverse(src_backup);
    p_sr_test_response ->head.next_addr    = FourBytesReverse(src_backup);
    p_sr_test_response ->head.length       = TwoBytesReverse(LORA_SR_RSSI_PAYLOAD_SIZE - 1);
    p_sr_test_response ->head.type         = FRAME_TYPE_LORA_SEND_RECV_TEST;
    
    p_sr_test_response ->payload.sr_test.env_rssi = env_rssi;
    p_sr_test_response ->payload.sr_test.recv_rssi = recv_rssi;
    
    check_sum = ByteGetCheckSum(&temp_buf[3],LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE - 2);
    p_sr_test_response ->payload.sr_test.checksum = TwoBytesReverse(check_sum);
    if(p_lora_send_func != (p_lora_send)0)
    {
        if(p_lora_send_func(temp_buf,LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE + 3) == X_True)
        {
            send_ok_cnt ++;
            LcdDisplayDistance(send_ok_cnt);
        }
    }
//////////    SysLogDebug(1,(" env_rssi : %d ; recv_rssi : %d \r\n",env_rssi,recv_rssi));
}
static X_Void Lora_SendRecvTest_Response(uint8_t *p_data,uint16_t length,
                        uint8_t lora_rela_channel,
                        p_lora_send send_func,X_Void (*get_rssi_req)(get_rssi_cb rssi_cb))
{
    sLoraCommonHeader * p_header = (sLoraCommonHeader *)p_data;
    relative_lora_channel = lora_rela_channel;
    if(send_func == (p_lora_send)0 || get_rssi_req == X_Null || length < LORA_DATA_HEADER_SIZE) {return;}
    
    p_lora_send_func = send_func;
    src_backup = p_header ->src_addr;
    dest_backup = p_header ->dest_addr;
    
    get_rssi_req(SR_Test_Receiver_LoadRssi);
}

static X_Boolean DoesLoraDataError(uint8_t *p_data,uint16_t length) // check the length and sum
{
	uint16_t check_sum1,check_sum2;
    if(p_data == (uint8_t *)0) {return X_True;}
	if(length < (LORA_DATA_HEADER_SIZE + 2) || length > MAX_LORA_RECV_DATA_LENGTH) 
	{
		SysLogDebug(1,("lora recv data length %d error\r\n",length));
		return X_True;
	} 
	check_sum1 = ByteGetCheckSum(p_data,length - 2);
	check_sum2 = (p_data[length-2] << 8) + p_data[length-1];
    
    sLoraCommonHeader *p_head = (sLoraCommonHeader *)p_data;
	if(check_sum1 != check_sum2) 
	{
////		SysLogDebug(1,(" %s lora recv data checksum error (%2x vs %2x); maybe type is %2x \r\n",
////                            (cur_instance == LI_Near) ? "near" : "relay",
////                            check_sum1,
////                            check_sum2,
////                            p_head ->type));
        
        SysLogDebug(1,(" lora recv data checksum error;maybe type :%2x \r\n",p_head ->type));
        
		return X_True;
	}
	return X_False;
}
/*************************************************************************/
#include "refresh_task_tag.h"

static uint8_t *p_data;
static uint16_t length;
static uint32_t src_addr_backup,lora_ms_backup;
static X_Boolean isNeedPowerUpDelay = X_True;
static X_Boolean isLoraConfigDone = X_False;

static X_Void LoraRecvHandle(uint32_t cur_ms);

static X_Void LoraModuleReConfig(uint32_t ms)
{
    ms = ms;
    lora_ms_backup = 0;
	sLoraConfig lora_config = LORA_CONFIG_DEFAULT_PARAM;
	
	RelayLoraInit();
	lora_config.module_addr = 0xffff;// TerminalNumberGet();
	lora_config.isRssiEnable = X_True;
	lora_config.channel      = GetRelativeLoraChannel(X_False);
	lora_config.air_speed    = GetRelativeLoraAirSpeed(X_False);
	SysLogDebug(1,(" ~~~~~~~~~~~~~ relay lora init , addr %2x , air speed %d , relative channel %d \r\n"
				,lora_config.module_addr,lora_config.air_speed,lora_config.channel));
	RelayLoraReset(&lora_config);
	RelayLora_EnableContinueReceive();
	isLoraConfigDone = X_False;
	isNeedPowerUpDelay = X_True;
    src_addr_backup    = 0;
}


TASK_BODY_BEGIN(lora_task,LoraModuleReConfig,3,0){
	X_if(isNeedPowerUpDelay == X_True)
	{
		//X_do(SysLogDebug(1,("delay 300 for lora power stable \r\n")));
		X_delay(300);
		X_do(isNeedPowerUpDelay = X_False;lora_ms_backup = x_task_param_ms); 
	}
    X_else
    {
        X_do(RelayLoraHandle();LoraRecvHandle(x_task_param_ms));
        X_if(isLoraConfigDone == X_False)
        {
            X_if(RelayLoraDoesBusy() != X_True)
            {
                X_do(isLoraConfigDone = X_True);
                 X_do(
                    RelayLora_SendRequest(dummy_buf,10);
                    RelayLora_SendRequest(dummy_buf,10);
                    );// send two dummy frame 
            }
            X_else
            {
                X_if((x_task_param_ms - lora_ms_backup) >= 10000)
                {
                    X_do(SysLogDebug(1,(" ~~~~~~~~~~~~~ !!! relay lora init failed, reinit \r\n")));
                    X_do(lora_ms_backup = x_task_param_ms);
                    X_return;
                }X_endif
            }X_endif
        }X_endif
    }X_endif
}TASK_BODY_END
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*************************************************************************/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static X_Void LoraRecvHandle(uint32_t cur_ms)
{
	uint32_t local_addr;
    
    if((cur_ms % 10) != 0) {return;}  
    if(RelayLoraDoesBusy() == X_True) {return;}
    if(RelayLoraDoesNewDataCome(&p_data,&length) != X_True) {return;}
	
    if(length >= 14)
	{
		#if (USE_LORA_DATA_RECV_DISPLAY_DEBUG != 0)
			SysLogDebug(1,(" relay lora recv data length %d \r\n",length));
			
			for(uint16_t i=0;i<length;i++)
			{
				SysLogDebug(1,("%2x ",p_data[i]));
			}
			SysLogDebug(1,("\r\n"));
			
		#endif
		
		sLoraCommonHeader * p_header = (sLoraCommonHeader *)p_data;
        p_header ->src_addr = FourBytesReverse(p_header ->src_addr);
        p_header ->dest_addr   = FourBytesReverse(p_header ->dest_addr);
        p_header ->next_addr   = FourBytesReverse(p_header ->next_addr);
            
		local_addr = TerminalNumberGet();
        
        if( p_header ->type == FRAME_TYPE_LORA_SEND_RECV_TEST && local_addr == p_header ->dest_addr)
        {
            SysLogDebug(1,(" lora sr test %d vs %d\r\n",
                        TwoBytesReverse(p_header ->length),
                        length));
            
            recv_ok_cnt ++;
            LcdDisplaylinenum(recv_ok_cnt);
            
            Lora_SendRecvTest_Response(p_data,length,GetRelativeLoraChannel(X_False),
                            RelayLora_SendRequest,RelayLoraGetRssiRequest);
            return;
        }
		else
		{
			SysLogDebug(1,(" not care \r\n"));
			return;
		}
	}
}

uint32_t GetBackupSrcAddr(X_Void)
{
    return src_addr_backup;
}
X_Boolean DoesRelayLoraConfigDone(X_Void)
{
	return isLoraConfigDone;
}

/***********************************************************************************/

static uint8_t relay_retry_cnt = 0;
static uint8_t cur_remote_env = 0,cur_remote_recv = 0;
static uint16_t send_recv_time_start_ms = 0;

static X_Void RecoveryTheRunState(X_Void)
{
    if(DoesUnderTestCondition() == X_True)
    {
        LcdDisplayStatus(TS_Run);
    }
    else
    {   
        if(isLoraConfigDone == X_True)
        {
            LcdDisplayStatus(TS_Stop);
        }
        else
        {
            LcdDisplayStatus(TS_Wait);
        }
        
    }
    
}

static X_Void SR_Test_LoadRssi(uint16_t env_rssi,uint16_t recv_rssi)
{
    LcdDisplayRssi(env_rssi,recv_rssi,cur_remote_env,cur_remote_recv);
    RecoveryTheRunState();
}

static X_Boolean relay_lora_recv_cb(e_cp_SendRecEvent event,uint8_t *p_data,uint16_t length,uint32_t ms)
{
	X_Boolean isOK = X_False;
	uint32_t local_device_addr;
	sLoraDataMultiParse * p_response = (sLoraDataMultiParse *)p_data;
	
    if(DoesUnderTestCondition() == X_False && isSingleTestOnce == X_False) {return X_True;}
    
	switch(event)
	{
		case CSRE_Successed:
			SysLogDebug(1,(" --------------- LE_Successed\r\n"));
			isOK = X_True;
            RelayLoraGetRssiRequest(SR_Test_LoadRssi);
            LcdDisplaySendRecvTime(mFunc_SoftTimerGetSysTickCnt() - send_recv_time_start_ms);
            isSingleTestOnce = X_False;
			break;
		case CSRE_Timeout:
			SysLogDebug(1,(" --------------- LE_TimeOut retry left %d \r\n",relay_retry_cnt));
		
			if(relay_retry_cnt > 0)
			{
				relay_retry_cnt --;
				RelayLoraSendAgain();
			}
            else
            {
                 LcdDisplayStatus(TS_Fail);
                 LcdDisplayRssi(0xff,0xff,0xff,0xff);
                 LcdDisplaySendRecvTime(5555);
                 DelayTodoReg(RecoveryTheRunState,2000);
                 isSingleTestOnce = X_False;
            }
			break;
		case CSRE_Error:
			SysLogDebug(1,(" --------------- LE_Failed\r\n"));
		
			if(relay_retry_cnt > 0)
			{
				relay_retry_cnt --;
				RelayLoraSendAgain();
			}
            else
            {
                 LcdDisplayStatus(TS_Fail);
                 LcdDisplayRssi(0xff,0xff,0xff,0xff);
                 LcdDisplaySendRecvTime(5555);
                 DelayTodoReg(RecoveryTheRunState,2500);
                 isSingleTestOnce = X_False;
            }
			break;
		case CSRE_RecvCheck:
				#if (USE_LORA_DATA_CHECK_DISPLAY_DEBUG != 0)
					SysLogDebug(1,(" LE_Check lora recv data length %d \r\n",length));
					for(uint16_t i=0;i<length;i++)
					{
						SysLogDebug(1,("%2x ",p_data[i]));
					}
					SysLogDebug(1,("\r\n"));
				#endif
				
				local_device_addr = TerminalNumberGet();
				if(FourBytesReverse(p_response ->head.next_addr) != local_device_addr) {return X_False;}
                
                if(DoesLoraDataError(p_data,length) == X_True) 
                {
                    LcdDisplayStatus(TS_Error);
                    DelayTodoReg(RecoveryTheRunState,2000);
                    return X_False;
                }
				
		
				if(length >= (LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE) 
                    && p_response ->head.type == FRAME_TYPE_LORA_SEND_RECV_TEST
					&& FourBytesReverse(p_response ->head.src_addr) == GetCurDestAddr() 
                    && FourBytesReverse(p_response ->head.dest_addr) == TerminalNumberGet()
					)
				{
                    cur_remote_env = p_response ->payload.sr_test.env_rssi;
                    cur_remote_recv = p_response ->payload.sr_test.recv_rssi;
					isOK = X_True; 
					SysLogDebug(1,(" --------------- LE_Check OK , get %2x ack \r\n",FRAME_TYPE_LORA_SEND_RECV_TEST));
                    recv_ok_cnt ++;
                    LcdDisplaylinenum(recv_ok_cnt);
				}
				
			break;
		default:
			break;
	}
	return isOK;
}

#define TEST_FRAME_EXT_SHORT_LENGTH            0
#define TEST_FRAME_EXT_MINDDLE_LENGTH          85
#define TEST_FRAME_EXT_LONG_LENGTH             170

X_Void Lora_SR_TestWithRssi(X_Void)
{
    uint8_t temp_buf[LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE + 3 + TEST_FRAME_EXT_LONG_LENGTH];
    uint16_t check_sum,i,ext_length;
    
    uint32_t dest_addr = GetCurDestAddr();
    
    temp_buf[0] = 0xab;
    temp_buf[1] = 0xcd;
    temp_buf[2] = GetRelativeLoraChannel(X_False);
    
    ext_length = TEST_FRAME_EXT_LONG_LENGTH;//TEST_FRAME_EXT_SHORT_LENGTH ; // TEST_FRAME_EXT_LONG_LENGTH;//TEST_FRAME_EXT_MINDDLE_LENGTH ; // TEST_FRAME_EXT_LONG_LENGTH;
    
    sLoraDataMultiParse * p_sr_test = (sLoraDataMultiParse *)&temp_buf[3];
    
//////    p_sr_test ->head.header[0]    = (LORA_CMD_FRAME_HEAD >> 8);
//////    p_sr_test ->head.header[1]    = (uint8_t)(LORA_CMD_FRAME_HEAD);
////    p_sr_test ->head.source_addr  = FourBytesReverse(TerminalNumberGet());
    p_sr_test ->head.dest_addr    = FourBytesReverse(dest_addr);
    p_sr_test ->head.next_addr    = FourBytesReverse(dest_addr);
    p_sr_test ->head.length       = TwoBytesReverse(ext_length);
    p_sr_test ->head.type         = FRAME_TYPE_LORA_SEND_RECV_TEST;
    
    for(i=0;i<ext_length;i++)
    {
        temp_buf[3 + LORA_DATA_HEADER_SIZE + i] = i;
    }
    
    
    check_sum = ByteGetCheckSum(&temp_buf[3],LORA_DATA_HEADER_SIZE + ext_length);
    temp_buf[3 + LORA_DATA_HEADER_SIZE + ext_length]     = check_sum >> 8;
    temp_buf[3 + LORA_DATA_HEADER_SIZE + 1 + ext_length] = check_sum;
    
    
    
    
    relay_retry_cnt = 0;
    if(RelayLora_SendRequestWithResponse(temp_buf,LORA_DATA_HEADER_SIZE + 2 + 3 + ext_length,
                        (cp_send_rec_cb)0,2200,relay_lora_recv_cb) == X_True)
    {
////////        LcdDisplayStatus(TS_Send);
        send_recv_time_start_ms = mFunc_SoftTimerGetSysTickCnt();
        
        send_ok_cnt ++;
        LcdDisplayDistance(send_ok_cnt);
    }
    else
    {
        LcdDisplayStatus(TS_Fail);
        DelayTodoReg(RecoveryTheRunState,2000);
        LcdDisplaySendRecvTime(5555);
    }
}


