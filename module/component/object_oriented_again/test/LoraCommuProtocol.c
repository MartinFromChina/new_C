#include "LoraCommuProtocol.h"
#include "../../../../../module/common/AppCommon.h"
#include "../../../../../module/common/Math/bit_operation.h"

sAllDeviceData p_AllData;
static eLoraInstance cur_instance = LI_Near;

#if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
#include  "mModule_4G.h"
#include "mApp_MainTask.h"
#include "relay_sys_info.h"
#include "DelayExtTask.h"
extern   X_Void SetBoardCastPeriod(X_Void);
extern   X_Void clearBoardCastState(X_Void);
extern   uint32_t mFunc_SoftTimerGetSysTickCnt(X_Void);
        
X_Boolean GateDevice_SendTestResult_ToClould(uint8_t *p_data,uint16_t length,X_Boolean isDirectlySend)
{
    sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
    uint8_t temp_buf[MAX_LORA_FRAME_SIZE + (sizeof(sCommonResponseExt)/sizeof(uint8_t))];
    
    if(p_data == (uint8_t *)0) {return X_False;}
    
    if(p_header ->type == FRAME_TYPE_TEXT_RESULT_REPORT && length == (LORA_DATA_HEADER_SIZE + LORA_AllDATA_PAYLOAD_SIZE)
		&& p_header ->header[0] == (LORA_RESP_FRAME_HEAD >> 8) && p_header ->header[1] == (LORA_RESP_FRAME_HEAD& 0x00FF))
	{
		SysLogDebug(1,(" going to trans single device test result to clould  \r\n"));
		
		sCommonResponseExt *p_report = (sCommonResponseExt *)temp_buf;
		
		HeaderAddrReverse(p_data);
		CommonHeaderLoad(MESSAGE_TYPE_RESPONSE_TO_CLOULD,&p_report ->header,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length);
		p_report ->opposite_message_or_config_type = COMMAND_TYPE_LORA_UPLOAD_TO_CLOULD;
		p_report ->err_code[0] = 0;
		p_report ->err_code[1] = 0;
		p_report ->reserve_length = TwoBytesReverse(length);
        
		CopyBuffer(p_data,&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],length);
		
        #if (DEVICE_TYPE == LPTT_Relay_Iap)
		return mModule_DTU4G_SendRequestWithCallBack(temp_buf,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length,(p_send_rec_cb)0); // fix it later , need send callback
        #endif
        
        #if (DEVICE_TYPE == LPTT_Relay)
        
        if(isDirectlySend == X_True)
        {
            return mModule_DTU4G_SendRequestWithCallBack(temp_buf,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length,(p_send_rec_cb)0);
        }
        
        Delay_TestResult_Request(temp_buf,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length
                                ,200 // adc sample may cost 160ms , so send it after 200 ms // 16次采样取平均值
                                ,X_True);
        return X_True;
        #endif
	}
    
    return X_False;
}

X_Boolean GateDevice_SendToClouldDirectly(uint8_t *p_data,uint16_t length)
{
    uint8_t temp_buf[MAX_LORA_FRAME_SIZE + (sizeof(sCommonResponseExt)/sizeof(uint8_t))];
    
    if(p_data == (uint8_t *)0) {return X_False;}
    SysLogDebug(1,(" going to trans data to clould  : length %d\r\n",length));
    
    sCommonResponseExt *p_report = (sCommonResponseExt *)temp_buf;
    
    HeaderAddrReverse(p_data);
    CommonHeaderLoad(MESSAGE_TYPE_RESPONSE_TO_CLOULD,&p_report ->header,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length);
    p_report ->opposite_message_or_config_type = COMMAND_TYPE_LORA_UPLOAD_TO_CLOULD;
    p_report ->err_code[0] = 0;
    p_report ->err_code[1] = 0;
    p_report ->reserve_length = TwoBytesReverse(length);
    
    CopyBuffer(p_data,&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],length);
    
    return mModule_DTU4G_SendRequestWithCallBack(temp_buf,sizeof(sCommonResponseExt)/sizeof(uint8_t) + length,(p_send_rec_cb)0); 
}

//uint8_t *p_data,uint16_t length boundary check is already done by DoesLoraDataError  
X_Boolean RelayReportDeviceConfig(uint8_t *p_data,uint16_t length)
{
    uint8_t temp_buf[MAX_LORA_FRAME_SIZE + (sizeof(sCommonResponseExt)/sizeof(uint8_t))];
    sCommonResponseExt *p_report = (sCommonResponseExt *)temp_buf;
    sLoraDataMultiParse * p_Multi = (sLoraDataMultiParse *)&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)];
    sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
    
    uint16_t temp_length;
    uint32_t local_addr,dest_addr,next_addr;
    s_RouteTable *p_cur_table = RouteTableGet();
    
    if(p_data == (uint8_t *)0 || p_cur_table == (s_RouteTable *)0) {return X_False;}
    
    if(p_header ->type == FRAME_TYPE_QUERY_DEVICE_CONFIG)
    {
        temp_length = LORA_DATA_HEADER_SIZE + LORA_SINGLE_CONFIG_PAYLOAD_SIZE;
        local_addr = TerminalNumberGet();
        dest_addr  = local_addr & 0xFFFFF000;  // gate addrerss
        next_addr  = p_cur_table ->addr_table[1]; // dest device number is 0 , the index is 0 + 1 = 1
        
        if(DoesAlsoGateDevice() == X_True)
        {
            CommonHeaderLoad(MESSAGE_TYPE_RESPONSE_TO_CLOULD,&p_report ->header,sizeof(sCommonResponseExt)/sizeof(uint8_t) + FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE);
            p_report ->opposite_message_or_config_type = COMMAND_TYPE_LORA_UPLOAD_TO_CLOULD;
            p_report ->err_code[0] = 0;
            p_report ->err_code[1] = 0;
            p_report ->reserve_length = TwoBytesReverse(temp_length); 
        }
        
        if(p_header ->header[0] == (LORA_CMD_FRAME_HEAD >> 8) && p_header ->header[1] == (LORA_CMD_FRAME_HEAD& 0x00FF))
        {
            p_Multi ->head.header[0]    = LORA_RESP_FRAME_HEAD >> 8;
            p_Multi ->head.header[1]    = (uint8_t)(LORA_RESP_FRAME_HEAD & 0x00FF);
            p_Multi ->head.source_addr  = FourBytesReverse(local_addr);
            p_Multi ->head.dest_addr    = FourBytesReverse(dest_addr);
            p_Multi ->head.next_addr    = FourBytesReverse(next_addr);
            p_Multi ->head.length       = TwoBytesReverse(LORA_SINGLE_CONFIG_PAYLOAD_SIZE - 1); // - 2for check sum + 1 for type 
            p_Multi ->head.type         = FRAME_TYPE_QUERY_DEVICE_CONFIG;
            
            LoadDeviceConfigToReport(&p_Multi ->payload.SingleConfig);
            p_Multi ->payload.SingleConfig.checksum = ByteGetCheckSum(&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],temp_length - 2);
            p_Multi ->payload.SingleConfig.checksum = TwoBytesReverse(p_Multi ->payload.SingleConfig.checksum);
            
            if(DoesAlsoGateDevice() == X_True)
            {
                mModule_DTU4G_SendRequest(temp_buf,
                sizeof(sCommonResponseExt)/sizeof(uint8_t) + temp_length); 
                SysLogDebug(1,(" gate device report config by 4G \r\n"));
            }
            else
            {
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 3] = local_addr >> 8;
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 2] = (uint8_t)local_addr;
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 1] = GetRelativeLoraChannel(X_False);
                RelayLora_SendRequest(&temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 3],temp_length + 3); 
                SysLogDebug(1,(" relay device report config by Relay Lora \r\n"));
            }
             return X_True;
        }
        else if(p_header ->header[0] == (LORA_RESP_FRAME_HEAD >> 8) && p_header ->header[1] == (LORA_RESP_FRAME_HEAD& 0x00FF))
        {
             if(DoesAlsoGateDevice() == X_True)
             {
                HeaderAddrReverse(p_data);
                CopyBuffer(p_data,&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],temp_length);
                mModule_DTU4G_SendRequest(temp_buf,
                sizeof(sCommonResponseExt)/sizeof(uint8_t) + temp_length); 
                SysLogDebug(1,(" gate device trans relay config by 4G \r\n"));
             }
//             else
//             {
//                // error 
//             }
             return X_True;
        }
        return X_False;
    }
    return X_False;
}


uint8_t  board_cast_frame_check_sum[2];

X_Void RelayLoraBroadcastFrameHandle(uint8_t *p_data,uint16_t length) 
{
    uint32_t sys_cnt;
    uint16_t delay_sec_to_send;
    sLoraDataMultiParse * p_Multi = (sLoraDataMultiParse *)p_data;
    
    if(board_cast_frame_check_sum[1] == p_data[length - 1] 
                && board_cast_frame_check_sum[0] == p_data[length - 2]) 
    {
        SysLogDebug(1,(" same boardcast frame again , drop it %d\r\n",mFunc_SoftTimerGetSysTickCnt()));
        return;
    }
    
    SysLogDebug(1,(" BroadcastFrame length %d vs %d, src %2x, dest %2x , next %2x type %2x \r\n"
            ,length, TwoBytesReverse(p_Multi ->head.length),
            p_Multi ->head.source_addr,p_Multi ->head.dest_addr,p_Multi ->head.next_addr
            ,p_Multi ->head.type));
    
    // set BoardCastState
    board_cast_frame_check_sum[1] = p_data[length - 1];
    board_cast_frame_check_sum[0] = p_data[length - 2];
    SetBoardCastPeriod();
    // clear BoardCastState after 30s
    DelayTodoReg(clearBoardCastState,30000);
    
    if(p_Multi ->head.type == FRAME_TYPE_SINGLE_DEVICE_CONFIG)
    {
        if(DoesUidMatched(p_Multi ->payload.SingleConfig.uid) == X_True)
        {
            SysLogDebug(1,(" boardcast config to me  \r\n"));
            #if 1
                SysInfoReConfig(&p_Multi ->payload.SingleConfig);
            #else
                #warning [!!! SysInfoReConfig need to be called here ]
            #endif
            return;
        }
        
        // not for me , trans it to other relay device once
        HeaderAddrReverse(p_data); 
        if(DoesAlsoGateDevice() == X_True)  // boardcast immediately 
        {
            SysLogDebug(1,(" boardcast immediately  \r\n"));
            RelayLoraBoardCastRequest(p_data,length);
        }
        else // random delay boardcast 
        {
            sys_cnt = mFunc_SoftTimerGetSysTickCnt();
            delay_sec_to_send = sys_cnt % MAX_BROADCAST_PERIOD_IN_SEC;
            delay_sec_to_send += 2;
            
            SysLogDebug(1,(" will trans after %d seconds , syscnt %d \r\n",delay_sec_to_send,sys_cnt));
            DelayLoraBoardcastRequest(p_data,length,delay_sec_to_send*1000);
        }
       
    }
    
    
}

#endif 


static uint8_t relative_lora_channel = 20;
static uint32_t src_backup,dest_backup;
static p_lora_send p_lora_send_func = (p_lora_send)0;
static X_Void SR_Test_LoadRssi(uint16_t env_rssi,uint16_t recv_rssi)
{
    uint8_t temp_buf[LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE + 3];
    uint16_t check_sum;
    temp_buf[0] = 0xab;
    temp_buf[1] = 0xcd;
    temp_buf[2] = relative_lora_channel;
    
    sLoraDataMultiParse * p_sr_test_response = (sLoraDataMultiParse *)&temp_buf[3];
    
    p_sr_test_response ->head.header[0]    = (LORA_RESP_FRAME_HEAD >> 8);
    p_sr_test_response ->head.header[1]    = (uint8_t)(LORA_RESP_FRAME_HEAD);
    p_sr_test_response ->head.source_addr  = FourBytesReverse(dest_backup);
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
        p_lora_send_func(temp_buf,LORA_SR_RSSI_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE + 3);
    }
//////////    SysLogDebug(1,(" env_rssi : %d ; recv_rssi : %d \r\n",env_rssi,recv_rssi));
}
X_Void Lora_SendRecvTest_Response(uint8_t *p_data,uint16_t length,
                        uint8_t lora_rela_channel,
                        p_lora_send send_func,X_Void (*get_rssi_req)(get_rssi_cb rssi_cb))
{
    sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
    relative_lora_channel = lora_rela_channel;
    if(send_func == (p_lora_send)0 || get_rssi_req == X_Null || length < LORA_DATA_HEADER_SIZE) {return;}
    
    p_lora_send_func = send_func;
    src_backup = p_header ->source_addr;
    dest_backup = p_header ->dest_addr;
    
    get_rssi_req(SR_Test_LoadRssi);
}

X_Boolean DoesLoraDataError(uint8_t *p_data,uint16_t length) // check the length and sum
{
	uint16_t check_sum1,check_sum2;
    if(p_data == (uint8_t *)0) {return X_True;}
	if(length < (LORA_DATA_HEADER_SIZE + 2) || length > MAX_LORA_RECV_DATA_LENGTH) 
	{
		SysLogDebug(1,(" %s lora recv data length %d error\r\n",(cur_instance == LI_Near) ? "near" : "relay",length));
		return X_True;
	} 
	check_sum1 = ByteGetCheckSum(p_data,length - 2);
	check_sum2 = (p_data[length-2] << 8) + p_data[length-1];
    
    sLoraDataHeader *p_head = (sLoraDataHeader *)p_data;
	if(check_sum1 != check_sum2) 
	{
////		SysLogDebug(1,(" %s lora recv data checksum error (%2x vs %2x); maybe type is %2x \r\n",
////                            (cur_instance == LI_Near) ? "near" : "relay",
////                            check_sum1,
////                            check_sum2,
////                            p_head ->type));
        
        SysLogDebug(1,(" %s lora recv data checksum error;maybe type :%2x \r\n",
                            (cur_instance == LI_Near) ? "near" : "relay",p_head ->type));
        
		return X_True;
	}
	return X_False;
}

X_Void HeaderAddrReverse(uint8_t *p_data)
{
	if(p_data == X_Null) {return;}
	sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
	p_header ->source_addr = FourBytesReverse(p_header ->source_addr);
	p_header ->dest_addr   = FourBytesReverse(p_header ->dest_addr);
	p_header ->next_addr   = FourBytesReverse(p_header ->next_addr);
}

eFrameFilterResult LoraFirstFilration(uint8_t *p_data,uint16_t length,uint32_t * p_local_addr_array,uint8_t array_length)
{
	u_AddrResolution local,dest,src;
	if(p_local_addr_array == X_Null) {return FFR_not_care;}
	
	sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
    
	if(array_length == 1)
	{
        dest.address  = p_header ->dest_addr;
//////////////////        SysLogDebug(1,("dest.address %2x dest.bit.device_num %2x \r\n",dest.address,dest.bit.device_num));
		if(dest.bit.device_num  == BROADCAST_DEVICE_NUMBER_MASK) {return FFR_boardcast;}
		if(p_header ->next_addr != p_local_addr_array[0]) {return FFR_not_care;}
		
		//SysLogDebug(1,(" src %2x dest %2x next %2x \r\n",p_header ->source_addr,p_header ->dest_addr,p_header ->next_addr));
		
		if(p_header ->dest_addr == p_local_addr_array[0]){return FFR_forMe;}
		
		local.address = p_local_addr_array[0];
		src.address   = p_header ->source_addr;
		
		local.bit.line_num = 0;local.bit.device_port = 0;
		dest.bit.line_num = 0;dest.bit.device_port = 0;
		src.bit.line_num = 0;src.bit.device_port = 0;
		
		if((local.address >= dest.address && local.address <= src.address) ||
		(local.address <= dest.address && local.address >= src.address))
        {
            if(p_header ->type == FRAME_TYPE_FILES_VERSION_REQUEST)
            {
                if(p_header ->header[0] == (uint8_t)(LORA_ACK_RESP_FRAME_HEAD >> 8) 
                        && p_header ->header[1] == (uint8_t)LORA_ACK_RESP_FRAME_HEAD)
                {
                    return FFR_trans;
                }
                else
                {
                    return FFR_trans; // no need ask the version 
//////                    return FFR_forMeAndTrans; //  ask the version when CMD head, response the version when resp head
                }
            }
            return FFR_trans;
        }
		
	}
	
	return FFR_not_care;
}

X_Boolean LoraNormalAck(uint8_t type,uint32_t src_addr,uint32_t dest_addr,s_RouteTable * p_table,
						uint8_t lora_relative_channel,p_lora_send send_func)
{
	uint8_t temp_buf[MAX_LORA_FRAME_SIZE + 3];
	uint32_t next_hoop_addr;
	u_AddrResolution local,dest;
    
	if(send_func == (p_lora_send)0) {return X_False;}
	
    local.address = src_addr;
    if(local.bit.line_num == 0) // relay device
    {
        dest.address = dest_addr;
        if(dest.bit.device_num >= 14) // todo : 跨链
        {
            SysLogDebug(1,(" param dest_addr %2x error , when LoraNormalAck \r\n",dest_addr));
            return X_False;
        }
        
        if(dest.bit.line_num == 0) // jump to relay device 
        {
            next_hoop_addr = p_table ->addr_table[dest.bit.device_num + 1];
        }
        else // jump to master device 
        {
            if(local.bit.device_num == dest.bit.device_num) // relay jump to master below 
            {
                next_hoop_addr = dest_addr;
            }
            else // relay jump to master which under another relay 
            {
                next_hoop_addr = p_table ->addr_table[dest.bit.device_num + 1];
            }
        }
    }
    else // master device 
    {
        local.bit.line_num = 0;
        next_hoop_addr = local.address;
    }
	
	temp_buf[0] = (src_addr >> 8); // 在监听模式下，LORA 发送地址已经没那么重要了。
	temp_buf[1] = src_addr;
	temp_buf[2] = lora_relative_channel;
	
	SysLogDebug(1,("going to ack , src %2x , dest %2x , next %2x , type %2x\r\n",src_addr,dest_addr,next_hoop_addr,type));
	
	sLoraNormalAckFrame *p_ack = (sLoraNormalAckFrame *)(&temp_buf[3]);
	
	p_ack ->head.header[0] = LORA_ACK_RESP_FRAME_HEAD >> 8;
	p_ack ->head.header[1] = (uint8_t)LORA_ACK_RESP_FRAME_HEAD;
	p_ack ->head.source_addr = FourBytesReverse(src_addr);//
	p_ack ->head.dest_addr   = FourBytesReverse(dest_addr);//
	p_ack ->head.next_addr   = FourBytesReverse(next_hoop_addr);
	p_ack ->head.length      = TwoBytesReverse(1);
	p_ack ->head.type        = type;
	
	p_ack ->checksum = ByteGetCheckSum(&temp_buf[3],(sizeof(sLoraNormalAckFrame)/sizeof(uint8_t)) - 2);
	p_ack ->checksum = TwoBytesReverse(p_ack ->checksum);
	return send_func(temp_buf,(sizeof(sLoraNormalAckFrame)/sizeof(uint8_t)) + 3);
}

#if (USE_DATA_LINK_TRANS_DEBUG != 0)
static X_Void DataLinkTransDisplay(u_AddrResolution local,u_AddrResolution src, 
                                    u_AddrResolution dest,u_AddrResolution next,uint8_t type,uint16_t length)
{
    X_Boolean isDirectionForward;
    X_Boolean isLocolInTheMiddle;
    X_Boolean isNextInTheMiddle;
    isDirectionForward = (dest.address >= src.address);

    SysLogDebug(1,("~~~~~~~type %2x,length %d ~~~~~ trans path below : \r\n",type,length));
    if(isDirectionForward == X_True)
    {
        if(local.bit.line_num == 0)
        {
            if(local.bit.device_num == src.bit.device_num)
            {
                
                SysLogDebug(1,(" {%d.%d} -->> ",local.bit.line_num,local.bit.device_num));
                isLocolInTheMiddle = X_False;
            
            }
            else if(local.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,("                                                    {%d.%d}",local.bit.line_num,local.bit.device_num)); // OK
                isLocolInTheMiddle = X_False;
            }
            else
            {
                SysLogDebug(1,("                  {%d.%d} -->> ",local.bit.line_num,local.bit.device_num)); // OK
                isLocolInTheMiddle = X_True;
            }
        }

        if(next.bit.line_num == 0)
        {
            if(next.bit.device_num == dest.bit.device_num)
            {        
                    if(isLocolInTheMiddle == X_True)
                    {
                                                 SysLogDebug(1,("                    (%d.%d) ",next.bit.line_num,next.bit.device_num));
                    }
                    else
                    {
                              SysLogDebug(1,("                                        [%d.%d] ",next.bit.line_num,next.bit.device_num)); // OK
                    }
                                                
            }
            else
            {
                    if(isLocolInTheMiddle == X_False)
                    {
                               SysLogDebug(1,("                                   (%d.%d) ",next.bit.line_num,next.bit.device_num)); // OK
                    }
                    else
                    {
                                                SysLogDebug(1,("  (%d.%d) ",next.bit.line_num,next.bit.device_num)); // OK
                    }
            }
        }
                SysLogDebug(1,("\r\n"));
                SysLogDebug(1,("   A                                                  | \r\n"));
                SysLogDebug(1,("   |                                                  V \r\n"));
                SysLogDebug(1,(" [%d.%d] -------------------------------------------- [%d.%d] \r\n\r\n\r\n"
                    ,src.bit.line_num,src.bit.device_num,dest.bit.line_num,dest.bit.device_num));

    }
    else
    {
        isNextInTheMiddle = X_False;
        if(next.bit.line_num == 0)
        {
            if(next.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,(" [%d.%d]  ",next.bit.line_num,next.bit.device_num)); // OK
                isNextInTheMiddle = X_False;
            }
            else
            {
                SysLogDebug(1,("                    [%d.%d]  ",next.bit.line_num,next.bit.device_num));
                isNextInTheMiddle = X_True;
            }
        }

        if(local.bit.line_num == 0)
        {
            if(local.bit.device_num == src.bit.device_num)
            {
                if(isNextInTheMiddle == X_True)
                {
                                                  SysLogDebug(1,("              {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
                }
                else
                {
                                SysLogDebug(1,("                                       <<-- {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
                }
               
            
            }
            else if(local.bit.device_num == dest.bit.device_num)
            {
                SysLogDebug(1,(" {%d.%d} ",local.bit.line_num,local.bit.device_num)); // OK
            }
            else
            {
                SysLogDebug(1,("       <<-- {%d.%d}  ",local.bit.line_num,local.bit.device_num)); // OK
            }
        }
                SysLogDebug(1,("\r\n"));
                SysLogDebug(1,("   |                                                  A \r\n"));
                SysLogDebug(1,("   V                                                  | \r\n"));
                SysLogDebug(1,(" [%d.%d] -------------------------------------------- [%d.%d] \r\n\r\n\r\n"
                   ,dest.bit.line_num,dest.bit.device_num,src.bit.line_num,src.bit.device_num));
    }
}
#endif

X_Boolean LoraFrameTrans(uint8_t *p_data,uint16_t length,
                            uint32_t local_addr,s_RouteTable * p_table,
                            uint8_t lora_relative_channel,p_lora_send send_func)// only used by relay device 
{
    uint32_t next_hop_addr;
	
	uint8_t temp_buf[MAX_LORA_FRAME_SIZE + 3];
	uint16_t check_sum;
    u_AddrResolution dest;
	
	if(length > MAX_LORA_FRAME_SIZE) 
	{
		SysLogDebug(1,("length error when LoraTransFrameHandle\r\n"));
		return X_False;
	}
	if(p_table == (s_RouteTable *)0 || send_func == (p_lora_send)0 || p_data == (uint8_t *)0)
    {
        SysLogDebug(1,("fatal error pointer null  \r\n"));
		return X_False;
    }
	
	temp_buf[0] = (local_addr >> 8);
	temp_buf[1] = local_addr;
	temp_buf[2] = lora_relative_channel;
	
	CopyBuffer(p_data,&temp_buf[3],length);
	sLoraDataMultiParse  * p_multi = (sLoraDataMultiParse  *)(&temp_buf[3]);
    
    dest.address = p_multi ->head.dest_addr;
    if( dest.bit.device_num <=14 ) // todo later : 跨链
    {
       next_hop_addr = p_table ->addr_table[dest.bit.device_num + 1];
    }
    else
    {
        SysLogDebug(1,("fatal error dest device number %d  \r\n",dest.bit.device_num));
		return X_False;
    }
	
	// ready for trans 
	p_multi ->head.next_addr = next_hop_addr;
	#if (USE_DATA_LINK_TRANS_DEBUG != 0)
	u_AddrResolution  local,src,t_dest,next;     
	local.address = local_addr;
	src.address   = p_multi ->head.source_addr;
	t_dest.address  = p_multi ->head.dest_addr;
	next.address  = next_hop_addr;
    
	DataLinkTransDisplay(local,src,t_dest,next,p_multi ->head.type,TwoBytesReverse(p_multi ->head.length));
	
	#endif
	HeaderAddrReverse(&temp_buf[3]);
	
	check_sum = ByteGetCheckSum(&temp_buf[3],length - 2);
	temp_buf[ length + 1] = check_sum >> 8;
	temp_buf[ length + 2] = check_sum;
	
//////	SysLogDebug(1,(" lora trans data length %d \r\n",length + 3));
//////	#ifdef USE_TDD_MOCK
//////		DisableDeviceInfoDisplay();
//////	#endif
//////	for(uint16_t i=0;i< (length + 3);i++)
//////	{
//////		SysLogDebug(1,("%2x ",temp_buf[i]));
//////	}
//////	SysLogDebug(1,("\r\n"));
//////	#ifdef USE_TDD_MOCK
//////		EnableDeviceInfoDisplay();
//////	#endif
	
	return send_func(temp_buf,length + 3);
}


X_Void SetCurLoraInstance(eLoraInstance instan)
{
    cur_instance = instan;
}

eLoraInstance GetCurLoraInstance(X_Void)
{
    return cur_instance;
}


/**********************************************************************/
/*******************************abut Iap , file system***************************************/
/**********************************************************************/
#include "../../BootloaderCommon/common/app_iap_info_share.h"
#include "../../BootloaderCommon/common/file_sys.h"
#if (DEVICE_TYPE == LPTT_Relay_Iap)
    #include "../../BootloaderCommon/receiver/before_recv.h"
#endif


extern X_Void mDri_MCU_Reset(X_Void);
extern X_Void mDri_SoftTimerDelayMs(uint32_t ms);
extern s_RouteTable * RouteTableGet(X_Void);

static sFileTree L_device_file_tree[4]; // // MAX_MASTER_DEVICE_NUM_WITH_ONE_RELAY    4

#if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
#include "const_loop_scheduler.h"
#include "mTask.h"
#include  "mModule_Lora.h"

//////static uint8_t cur_device_qty = 0,cur_rela_channel = 76;//(DEFAULT_NEAR_LORA_CHANNEL - 410);
//////static uint32_t cur_work_period = 0,syn_task_work_ms = 0,cur_local_addr = 0x00438010; // fix it later , the magic number 
static X_Boolean isGetDeviceInfo = X_False;
static uint16_t  L_device_cur_use_file_version[4][3]; // L means lower device ;   3 response to : master , wave , fpga 

static X_Boolean DoesOtherTaskBusy(X_Void)
{
    #if (DEVICE_TYPE == LPTT_Relay_Iap)
        return DoesFileDownloadAlreadyBegin();
    #else
        return X_False;
    #endif
}

static X_Boolean files_version_recv_cb(eLoraEvent event,uint8_t *p_data,uint16_t length)
{
	X_Boolean isOK = X_False;
    uint8_t cur_line_num;
	uint32_t local_device_addr;
    u_AddrResolution src;

	sLoraDataMultiParse *p_response = (sLoraDataMultiParse *)p_data;
	switch(event)
	{
		case LE_Successed:
			SysLogDebug(1,(" --------------- files_version LE_Successed \r\n"));
			isOK = X_True;
            isGetDeviceInfo = X_True;
			break;
		case LE_TimeOut:
			SysLogDebug(1,(" --------------- files_version LE_TimeOut \r\n"));
			break;
		case LE_Failed:
			SysLogDebug(1,(" --------------- files_version LE_Failed\r\n"));
			break;
		case LE_Check:
//////////            SysLogDebug(1,(" --------------- files_version LE_Check\r\n"));
				#if (USE_LORA_DATA_CHECK_DISPLAY_DEBUG != 0)
					SysLogDebug(1,(" files_version LE_Check lora recv data length %d \r\n",length));
					for(uint16_t i=0;i<length;i++)
					{
						SysLogDebug(1,("%2x ",p_data[i]));
					}
					SysLogDebug(1,("\r\n"));
				#endif

				if(DoesLoraDataError(p_data,length) == X_True) {return X_False;}
				local_device_addr = (DoesBootSysInfoValid() == X_True) ? GetBootTerminalNUmber():TerminalNumberGet();
				if(FourBytesReverse(p_response ->head.next_addr) != local_device_addr) {return X_False;}
//////		
//////				SysLogDebug(0,("type %2x vs %2x, src_addr %2x vs %2x ; dest_addr %2x vs %2x ; length %d vs %d\r\n"
//////						,p_ack ->head.type
//////						,FourBytesReverse(p_ack ->head.source_addr),near_dest_addr
//////						,FourBytesReverse(p_ack ->head.dest_addr),near_local_addr
//////						,length,(uint16_t)(sizeof(sLoraNormalAckFrame)/sizeof(uint8_t))));
//////					
				if(length == (FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE) && p_response ->head.type == FRAME_TYPE_FILES_VERSION_REQUEST
					&& p_response ->head.header[0] == (uint8_t)(LORA_RESP_FRAME_HEAD >> 8) && p_response ->head.header[1] == (uint8_t)LORA_RESP_FRAME_HEAD
					&& FourBytesReverse(p_response ->head.dest_addr) == local_device_addr
					)
				{
					isOK = X_True; 
					
                    // load files version 
                    src.address = FourBytesReverse(p_response ->head.source_addr);
                    cur_line_num = src.bit.line_num;
                    
                    if(cur_line_num >= 1 && cur_line_num <= 4)
                    {
                        SysLogDebug(1,(" --------------- LE_Check OK , get file version response ,device line %d\r\n",cur_line_num));
          
                        L_device_cur_use_file_version[cur_line_num - 1][0] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].master[0]);
                        L_device_cur_use_file_version[cur_line_num - 1][1] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].wave[0]);
                        L_device_cur_use_file_version[cur_line_num - 1][2] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].fpga[0]);
                        
                        SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" --------------- master %2x; wave %2x; fpga %2x\r\n"
                                        ,L_device_cur_use_file_version[cur_line_num - 1][0]
                                        ,L_device_cur_use_file_version[cur_line_num - 1][1]
                                        ,L_device_cur_use_file_version[cur_line_num - 1][2]));
                        
                        for(uint8_t j=0;j<(MAX_VERSION_FOR_ONE_TYPE_FILE);j++)
                        {
                            L_device_file_tree[cur_line_num - 1].version_array[0][j] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].relay[j+1]);
                            L_device_file_tree[cur_line_num - 1].version_array[1][j] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].master[j+1]);
                            L_device_file_tree[cur_line_num - 1].version_array[2][j] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].wave[j+1]);
                            L_device_file_tree[cur_line_num - 1].version_array[3][j] = TwoBytesReverse(p_response ->payload.FileVersion.m_device[0].fpga[j+1]); 
                        }
                    }
				}
				
			break;
		default:
			break;
	}
	return isOK;
}
/*
static X_Void QueryMasterBasicInfo(uint8_t line_cnt) 
{
    u_AddrResolution local;
    uint16_t check_sum;
    uint8_t temp_buf[LORA_DATA_HEADER_SIZE + 5]; // 3 for header , 2 for check sum
    
    sLoraDataCommon *p_cmd = (sLoraDataCommon *)&temp_buf[3];
    
    local.address = cur_local_addr;
    local.bit.line_num = line_cnt;
    
    temp_buf[0] = (cur_local_addr >> 8); // 在监听模式下，LORA 发送地址已经没那么重要了。
	temp_buf[1] = cur_local_addr;
	temp_buf[2] = cur_rela_channel;
    
    p_cmd ->head.header[0] = LORA_CMD_FRAME_HEAD>>8;
    p_cmd ->head.header[1] = (uint8_t)LORA_CMD_FRAME_HEAD;
    p_cmd ->head.source_addr = FourBytesReverse(cur_local_addr);
    p_cmd ->head.dest_addr   = FourBytesReverse(local.address);
    p_cmd ->head.next_addr   = FourBytesReverse(local.address);
    p_cmd ->head.length      = TwoBytesReverse(1);
    p_cmd ->head.type        = FRAME_TYPE_FILES_VERSION_REQUEST;
    
    check_sum = ByteGetCheckSum(&temp_buf[3],LORA_DATA_HEADER_SIZE);
    p_cmd ->payload[0] = check_sum >> 8;
    p_cmd ->payload[1] = check_sum;
    
    NearLora_SendRequestWithResponse(temp_buf,LORA_DATA_HEADER_SIZE + 5,
                                     (p_lora_send_rec_cb)0,2000,files_version_recv_cb);
    
}
*/

X_Boolean DoesGetActiveFilesVersionReportFromMasterDevice(uint8_t *p_data, uint16_t length)
{
    return files_version_recv_cb(LE_Check,p_data,length);
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**********************************************************************/
#endif

/**********************************************************************
***********************************************************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
        typedef enum{
        CFT_relay = 0,
        CFT_master,
        CFT_wave,
        CFT_fpga,
        }eCompressedFileType;
      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/   
extern uint32_t TerminalNumberGet(X_Void);
static X_Boolean ConvertFileVersionNumToFileString(uint8_t type,uint16_t version,char *p_file_name)
{
    uint8_t cur_length;
    SysLogDebug(1,(" type is %2x \r\n",type));
    if(type > CFT_fpga || p_file_name == (char *)0) {return X_False;} 
    
    switch(type)
    {
        case CFT_relay:        // relay
            sprintf(&p_file_name[0],"%s","MCUrV");
            break;
        case CFT_master:        // master
            sprintf(&p_file_name[0],"%s","MCUmV");
            break;
        case CFT_wave:        // wave
            sprintf(&p_file_name[0],"%s","MCUwV");
            break;
        case CFT_fpga:        // fpga 
            sprintf(&p_file_name[0],"%s","FPGAV");
            break;
        default:
            // error 
            break;
    }
    cur_length = strlen(p_file_name);
    if((cur_length + 4) > MAX_FILE_NAME_LENGTH) {return X_False;}
    sprintf(&p_file_name[cur_length],"%d.",version >> 8);  // 以后试试用snprintf
    
    cur_length = strlen(p_file_name);
    if((cur_length + 3) > MAX_FILE_NAME_LENGTH) {return X_False;}
    sprintf(&p_file_name[cur_length],"%d.",(version & 0x00FF)/10);
    
    cur_length = strlen(p_file_name);
    if((cur_length + 2) > MAX_FILE_NAME_LENGTH) {return X_False;}
    sprintf(&p_file_name[strlen(p_file_name)],"%d",(version & 0x00FF)%10);
    
    return X_True;
}

X_Void FilesVersionInit(X_Void)
{
    uint8_t i,j;
    for(i=0;i<4;i++) // MAX_MASTER_DEVICE_NUM_WITH_ONE_RELAY    4
    {
        #if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
            L_device_cur_use_file_version[i][0] = 0xffff;  // 0 : master 
            L_device_cur_use_file_version[i][1] = 0xffff;  // 1 : wave 
            L_device_cur_use_file_version[i][2] = 0xffff;  // 2 : fpga 
        #endif
        for(j=0;j<MAX_VERSION_FOR_ONE_TYPE_FILE;j++)
        {
            L_device_file_tree[i].version_array[CFT_relay][j] = 0xffff;
            L_device_file_tree[i].version_array[CFT_master][j] = 0xffff;
            L_device_file_tree[i].version_array[CFT_wave][j] = 0xffff;
            L_device_file_tree[i].version_array[CFT_fpga][j] = 0xffff;
        }
    }
}

static uint16_t GenerateLoraResponseData_AboutFilesVersion(uint8_t *p_temp_buf,uint8_t *p_cmd)
{
    uint32_t next_hoop_addr;
	u_AddrResolution local,dest;
    
    sFileTree *p_tree,tree;
    uint32_t local_addr;
    uint8_t i,j,device_scope;
    uint16_t check_sum;
    uint16_t cur_firmware_version;
    
    sLoraDataMultiParse *p_multi    = (sLoraDataMultiParse *)p_temp_buf;
    sLoraDataHeader *p_command      = (sLoraDataHeader *)p_cmd;
    s_RouteTable *p_table           = RouteTableGet();
    
    if(p_temp_buf == (uint8_t *)0 || p_cmd == (uint8_t *)0 || p_table == (s_RouteTable *)0) {return 0;}
    
	local_addr = (DoesBootSysInfoValid() == X_True) ? GetBootTerminalNUmber():TerminalNumberGet();
	/***********************************************/ 
    local.address = local_addr;
    if(local.bit.line_num == 0) // relay device
    {
        dest.address = p_command ->source_addr;
        if(dest.bit.device_num >= 14) // todo : 跨链
        {
            SysLogDebug(1,(" param dest_addr %2x error , when ResponseData_AboutFilesVersion \r\n",dest.address));
            return X_False;
        }
        
        if(dest.bit.line_num == 0) // jump to relay device 
        {
            next_hoop_addr = p_table ->addr_table[dest.bit.device_num + 1];
        }
        else // jump to master device 
        {
            SysLogDebug(1,(" param dest_addr %2x error , can not ResponseData_AboutFilesVersion to master device \r\n",dest.address));
            return X_False;
//////////            if(local.bit.device_num == dest.bit.device_num) // relay jump to master below 
//////////            {
//////////                next_hoop_addr = dest_addr;
//////////            }
//////////            else // relay jump to master which under another relay 
//////////            {
//////////                next_hoop_addr = p_table ->addr_table[dest.bit.device_num + 1];
//////////            }
        }
    }
    else // master device 
    {
        local.bit.line_num = 0;
        next_hoop_addr = local.address;
    }
    /***********************************************/ 
    // fix it later 
    p_multi ->head.header[0] = LORA_RESP_FRAME_HEAD >> 8;
    p_multi ->head.header[1] = (uint8_t)LORA_RESP_FRAME_HEAD;
    p_multi ->head.source_addr = FourBytesReverse(local_addr);
    p_multi ->head.dest_addr = FourBytesReverse(p_command ->source_addr);
    p_multi ->head.next_addr = FourBytesReverse(next_hoop_addr);
    p_multi ->head.length    = TwoBytesReverse(FILE_VERSION_PAYLOAD_SIZE - 1); // -2 (checksum)  + 1(type)
    p_multi ->head.type      = FRAME_TYPE_FILES_VERSION_REQUEST;
    
    cur_firmware_version = CPU_FIRMWARE_VERSION;
    
    #if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
        device_scope = 5;
    #else
        device_scope = 1;
    #endif
    
    for(i=0;i<device_scope;i++)
    {
        if(i == 0) // local device 
        {
            if(GetFileTree(&tree) != X_True) {return 0;}
            p_tree = &tree;
        }
        else // i : 1 ~ 4 ; lower device  // for master device , the data is no meaning when i != 0
        {
            p_tree = &L_device_file_tree[i-1];
        }
        
        #if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
            if(i == 0)
            {
                p_multi ->payload.FileVersion.m_device[i].relay[0]  = TwoBytesReverse(cur_firmware_version);
                p_multi ->payload.FileVersion.m_device[i].master[0] = 0xffff;
                p_multi ->payload.FileVersion.m_device[i].wave[0]   = 0xffff;
                p_multi ->payload.FileVersion.m_device[i].fpga[0]   = 0xffff;
            }
            else
            {
                p_multi ->payload.FileVersion.m_device[i].relay[0]  = 0xffff;
                p_multi ->payload.FileVersion.m_device[i].master[0] = TwoBytesReverse(L_device_cur_use_file_version[i-1][0]);
                p_multi ->payload.FileVersion.m_device[i].wave[0]   = TwoBytesReverse(L_device_cur_use_file_version[i-1][1]);
                p_multi ->payload.FileVersion.m_device[i].fpga[0]   = TwoBytesReverse(L_device_cur_use_file_version[i-1][2]);
            }
            
        #else // i == 0
            p_multi ->payload.FileVersion.m_device[i].relay[0]  = 0xffff;
            p_multi ->payload.FileVersion.m_device[i].master[0] = TwoBytesReverse(cur_firmware_version);
            p_multi ->payload.FileVersion.m_device[i].wave[0]   = TwoBytesReverse(0x0100);
            p_multi ->payload.FileVersion.m_device[i].fpga[0]   = TwoBytesReverse(0x0100);
        #endif
        
        for(j=1;j<(MAX_VERSION_FOR_ONE_TYPE_FILE + 1);j++)
        {
            p_multi ->payload.FileVersion.m_device[i].relay[j]  = TwoBytesReverse(p_tree ->version_array[0][j-1]);
            p_multi ->payload.FileVersion.m_device[i].master[j] = TwoBytesReverse(p_tree ->version_array[1][j-1]);
            p_multi ->payload.FileVersion.m_device[i].wave[j]   = TwoBytesReverse(p_tree ->version_array[2][j-1]);
            p_multi ->payload.FileVersion.m_device[i].fpga[j]   = TwoBytesReverse(p_tree ->version_array[3][j-1]);
        }
    }
    #if (FILES_VERSION_REPORT_DEBUG != 0 )
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" --------------- the final report files version is \r\n"));
    
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" relay: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[0].relay[0] & 0x0F,p_multi ->payload.FileVersion.m_device[0].relay[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[0].relay[1] & 0x0F,p_multi ->payload.FileVersion.m_device[0].relay[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[0].relay[2] & 0x0F,p_multi ->payload.FileVersion.m_device[0].relay[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[0].relay[3] & 0x0F,p_multi ->payload.FileVersion.m_device[0].relay[3] >> 8
                ));
    // master 1
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" master1: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[1].master[0] & 0x0F,p_multi ->payload.FileVersion.m_device[1].master[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].master[1] & 0x0F,p_multi ->payload.FileVersion.m_device[1].master[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].master[2] & 0x0F,p_multi ->payload.FileVersion.m_device[1].master[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].master[3] & 0x0F,p_multi ->payload.FileVersion.m_device[1].master[3] >> 8
                ));
    
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- wave1: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[1].wave[0] & 0x0F,p_multi ->payload.FileVersion.m_device[1].wave[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].wave[1] & 0x0F,p_multi ->payload.FileVersion.m_device[1].wave[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].wave[2] & 0x0F,p_multi ->payload.FileVersion.m_device[1].wave[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].wave[3] & 0x0F,p_multi ->payload.FileVersion.m_device[1].wave[3] >> 8
                ));
                
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- fpga1: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[1].fpga[0] & 0x0F,p_multi ->payload.FileVersion.m_device[1].fpga[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].fpga[1] & 0x0F,p_multi ->payload.FileVersion.m_device[1].fpga[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].fpga[2] & 0x0F,p_multi ->payload.FileVersion.m_device[1].fpga[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[1].fpga[3] & 0x0F,p_multi ->payload.FileVersion.m_device[1].fpga[3] >> 8
                ));
    
    // master 2
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" master2: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[2].master[0] & 0x0F,p_multi ->payload.FileVersion.m_device[2].master[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].master[1] & 0x0F,p_multi ->payload.FileVersion.m_device[2].master[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].master[2] & 0x0F,p_multi ->payload.FileVersion.m_device[2].master[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].master[3] & 0x0F,p_multi ->payload.FileVersion.m_device[2].master[3] >> 8
                ));
    
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- wave2: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[2].wave[0] & 0x0F,p_multi ->payload.FileVersion.m_device[2].wave[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].wave[1] & 0x0F,p_multi ->payload.FileVersion.m_device[2].wave[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].wave[2] & 0x0F,p_multi ->payload.FileVersion.m_device[2].wave[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].wave[3] & 0x0F,p_multi ->payload.FileVersion.m_device[2].wave[3] >> 8
                ));
                
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- fpga2: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[2].fpga[0] & 0x0F,p_multi ->payload.FileVersion.m_device[2].fpga[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].fpga[1] & 0x0F,p_multi ->payload.FileVersion.m_device[2].fpga[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].fpga[2] & 0x0F,p_multi ->payload.FileVersion.m_device[2].fpga[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[2].fpga[3] & 0x0F,p_multi ->payload.FileVersion.m_device[2].fpga[3] >> 8
                ));
                
    // master 3            
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" master3: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[3].master[0] & 0x0F,p_multi ->payload.FileVersion.m_device[3].master[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].master[1] & 0x0F,p_multi ->payload.FileVersion.m_device[3].master[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].master[2] & 0x0F,p_multi ->payload.FileVersion.m_device[3].master[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].master[3] & 0x0F,p_multi ->payload.FileVersion.m_device[3].master[3] >> 8
                ));
    
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- wave3: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[3].wave[0] & 0x0F,p_multi ->payload.FileVersion.m_device[3].wave[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].wave[1] & 0x0F,p_multi ->payload.FileVersion.m_device[3].wave[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].wave[2] & 0x0F,p_multi ->payload.FileVersion.m_device[3].wave[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].wave[3] & 0x0F,p_multi ->payload.FileVersion.m_device[3].wave[3] >> 8
                ));
                
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- fpga3: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[3].fpga[0] & 0x0F,p_multi ->payload.FileVersion.m_device[3].fpga[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].fpga[1] & 0x0F,p_multi ->payload.FileVersion.m_device[3].fpga[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].fpga[2] & 0x0F,p_multi ->payload.FileVersion.m_device[3].fpga[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[3].fpga[3] & 0x0F,p_multi ->payload.FileVersion.m_device[3].fpga[3] >> 8
                ));
                
    // master 4            
    SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" master4: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[4].master[0] & 0x0F,p_multi ->payload.FileVersion.m_device[4].master[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].master[1] & 0x0F,p_multi ->payload.FileVersion.m_device[4].master[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].master[2] & 0x0F,p_multi ->payload.FileVersion.m_device[4].master[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].master[3] & 0x0F,p_multi ->payload.FileVersion.m_device[4].master[3] >> 8
                ));
    
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- wave1: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[4].wave[0] & 0x0F,p_multi ->payload.FileVersion.m_device[4].wave[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].wave[1] & 0x0F,p_multi ->payload.FileVersion.m_device[4].wave[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].wave[2] & 0x0F,p_multi ->payload.FileVersion.m_device[4].wave[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].wave[3] & 0x0F,p_multi ->payload.FileVersion.m_device[4].wave[3] >> 8
                ));
                
     SysLogDebug(FILES_VERSION_REPORT_DEBUG,(" ----- fpga1: %d.%d  ; %d.%d  ; %d.%d   ; %d.%d ;\r\n"
                ,p_multi ->payload.FileVersion.m_device[4].fpga[0] & 0x0F,p_multi ->payload.FileVersion.m_device[4].fpga[0] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].fpga[1] & 0x0F,p_multi ->payload.FileVersion.m_device[4].fpga[1] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].fpga[2] & 0x0F,p_multi ->payload.FileVersion.m_device[4].fpga[2] >> 8
                ,p_multi ->payload.FileVersion.m_device[4].fpga[3] & 0x0F,p_multi ->payload.FileVersion.m_device[4].fpga[3] >> 8
                ));
    #endif

    /***********************************************/
    check_sum = ByteGetCheckSum(p_temp_buf,FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE - 2); // fix it later 
    p_multi ->payload.FileVersion.checksum = TwoBytesReverse(check_sum);
    
    return (FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE);
}


static X_Boolean ProgramCertainFileHandle(uint8_t * p_data,uint16_t length)
{
    s_IapOperation temp_operation;
    sLoraDataMultiParse *p_prog_file = (sLoraDataMultiParse *)p_data;
    
    if(p_data == (uint8_t *)0 ) {return X_False;} // || length < 
    
    temp_operation.operation = IO_program;
    byteBufInit(temp_operation.param.file_name,MAX_FILE_NAME_LENGTH,'\0');
    if(ConvertFileVersionNumToFileString(p_prog_file ->payload.ProgramFile.type,
                                            TwoBytesReverse(p_prog_file ->payload.ProgramFile.version),
                                            (char*)temp_operation.param.file_name) != X_True)
    {
        return X_False;
    }
    
    if(BootSetOperation(&temp_operation) == X_True)
    {
        // delay ? 
        mDri_MCU_Reset();
    }
    else
    {
        return X_False;
    }
    return X_True;    
}



#if (DEVICE_TYPE == LPTT_Relay_Iap || DEVICE_TYPE == LPTT_Relay)
    #include "DelayExtTask.h"
    #include "Clould_module_Interface.h"

    static X_Boolean isIapDeviceBusyInTheLink = X_False;
    static X_Boolean TransCertainFileHandle(uint8_t * p_data,uint16_t length)
    {
        sLoraDataMultiParse *p_parse = (sLoraDataMultiParse *)p_data;
        
        s_IapOperation temp_operation;
        if(p_data == (uint8_t *)0 ) {return X_False;} // || length <    // no need pointer check , the only caller is func: IapCommandHandle  ? 
        
        temp_operation.operation = IO_trans;
        temp_operation.param.t_info.dest_addr = FourBytesReverse(p_parse ->payload.FileTrans.file_dest_addr);
        byteBufInit(temp_operation.param.t_info.file,MAX_FILE_NAME_LENGTH,'\0');
        
        if(ConvertFileVersionNumToFileString(p_parse ->payload.FileTrans.file_type,
                                                TwoBytesReverse(p_parse ->payload.FileTrans.file_version),
                                                (char*)temp_operation.param.t_info.file) != X_True)
        {
            return X_False;
        }
        
        SysLogDebug(1,(" \r\n \r\n --------- going to trans file %s to device %2x  \r\n "
                            ,temp_operation.param.t_info.file,temp_operation.param.t_info.dest_addr));
        
        if(BootSetOperation(&temp_operation) == X_True)
        {
            mDri_SoftTimerDelayMs(500);
            mDri_MCU_Reset();
        }
        else
        {
            return X_False;
        }
        return X_True;
    }

    extern X_Boolean mModule_DTU4G_SendRequest(const uint8_t *p_data,uint16_t length);

    static X_Void ClearIapDeviceBusyState(X_Void)
    {
        isIapDeviceBusyInTheLink = X_False;
    }
    static X_Void SetIapDeviceBusyState(X_Void)
    {
        isIapDeviceBusyInTheLink = X_True;
        log_state_clear_TodoReg(ClearIapDeviceBusyState,50000);
    }
    
        
    X_Boolean Does_AnIapDeviceBusyInTheLink(X_Void)
    {
        return isIapDeviceBusyInTheLink;
    }
    
    X_Boolean IapCommandHandle(uint8_t * p_data,uint16_t length,uint32_t local_address)
    {
        uint16_t temp_length;
        uint8_t  temp_buf[MAX_LORA_FRAME_SIZE + (sizeof(sCommonResponseExt)/sizeof(uint8_t))];
        
        sLoraDataHeader * p_header   = (sLoraDataHeader *)p_data;
        sCommonResponseExt *p_report = (sCommonResponseExt *)temp_buf;
        
        if(length < (LORA_DATA_HEADER_SIZE + 2) || length > MAX_LORA_RECV_DATA_LENGTH) {return X_False;}
        if(p_data == (uint8_t *)0) {return X_False;}
        
        if(p_header ->type == FRAME_TYPE_PROGRAM_CERTAIN_FILE)
        {
            SetIapDeviceBusyState();
            if(ProgramCertainFileHandle(p_data,length) != X_True)
            {
                SysLogDebug(1,(" fatal error when get cmd : program certain file \r\n"));
            }
            return X_True;
        }
        else if(p_header ->type == FRAME_TYPE_FILE_TRANS_TO_NEIGHBOR
            && p_header ->header[0] == (LORA_CMD_FRAME_HEAD >> 8) && p_header ->header[1] == (uint8_t)(LORA_CMD_FRAME_HEAD& 0x00FF))
        {
             SetIapDeviceBusyState();
            if(TransCertainFileHandle(p_data,length) != X_True)
            {
                SysLogDebug(1,(" fatal error when get cmd : trans certain file \r\n"));
            }
            return X_True;
        }
        else if(p_header ->type == FRAME_TYPE_FILES_VERSION_REQUEST
            && p_header ->header[0] == (LORA_CMD_FRAME_HEAD >> 8) && p_header ->header[1] == (uint8_t)(LORA_CMD_FRAME_HEAD& 0x00FF))
        { 
//////            SetIapDeviceBusyState();
             
            temp_length = GenerateLoraResponseData_AboutFilesVersion(&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],p_data);
           
            if(temp_length == 0 || temp_length > MAX_LORA_FRAME_SIZE)
            {
                SysLogDebug(1,(" fatal length error %d when report FILES_VERSION \r\n",temp_length));
            }
            
            if(DoesAlsoGateDevice() == X_True)
            {
                CommonHeaderLoad(MESSAGE_TYPE_RESPONSE_TO_CLOULD,&p_report ->header,sizeof(sCommonResponseExt)/sizeof(uint8_t) + FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE);
                p_report ->opposite_message_or_config_type = COMMAND_TYPE_LORA_UPLOAD_TO_CLOULD;
                p_report ->err_code[0] = 0;
                p_report ->err_code[1] = 0;
                p_report ->reserve_length = TwoBytesReverse(temp_length); 
                
                mModule_DTU4G_SendRequest(temp_buf,
                    sizeof(sCommonResponseExt)/sizeof(uint8_t) + temp_length); 
            }
            else 
            {
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 3] = local_address >> 8;
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 2] = (uint8_t)local_address;
                temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 1] = GetRelativeLoraChannel(X_False);
                RelayLora_SendRequest(&temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 3],temp_length + 3); 
                
    //////            SysLogDebug(1,(" relay lora response length %d \r\n",temp_length + 3));
    //////            for(uint16_t i=0;i< (temp_length + 3);i++)
    //////            {
    //////                SysLogDebug(1,("%2x ",temp_buf[(sizeof(sCommonResponseExt)/sizeof(uint8_t)) - 3 + i]));
    //////            }
    //////            SysLogDebug(1,("\r\n"));

            }
            return X_True;
        }
        else if(p_header ->type == FRAME_TYPE_FILES_VERSION_REQUEST
            && p_header ->header[0] == (LORA_RESP_FRAME_HEAD >> 8) && p_header ->header[1] == (uint8_t)(LORA_RESP_FRAME_HEAD& 0x00FF))
        {
//////////////             SetIapDeviceBusyState(); // not a process ,no need set busy state 
            if(DoesAlsoGateDevice() == X_True)
            {
                SysLogDebug(1,(" going to trans files version to clould  \r\n"));
                
                temp_length = FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE;
                HeaderAddrReverse(p_data);
                CommonHeaderLoad(MESSAGE_TYPE_RESPONSE_TO_CLOULD,&p_report ->header,sizeof(sCommonResponseExt)/sizeof(uint8_t) + FILE_VERSION_PAYLOAD_SIZE + LORA_DATA_HEADER_SIZE);
                p_report ->opposite_message_or_config_type = COMMAND_TYPE_LORA_UPLOAD_TO_CLOULD;
                p_report ->err_code[0] = 0;
                p_report ->err_code[1] = 0;
                p_report ->reserve_length = TwoBytesReverse(temp_length); 
                CopyBuffer(p_data,&temp_buf[sizeof(sCommonResponseExt)/sizeof(uint8_t)],temp_length);
                
                mModule_DTU4G_SendRequest(temp_buf,
                    sizeof(sCommonResponseExt)/sizeof(uint8_t) + temp_length); 
            }
            else
            {
                SysLogDebug(1,("sth wrong , files version response to relay device ? \r\n"));
            }
            return X_True;
        }
        else if(p_header ->type == FRAME_TYPE_LOG_REPORT
            && p_header ->header[0] == (LORA_RESP_FRAME_HEAD >> 8) && p_header ->header[1] == (uint8_t)(LORA_RESP_FRAME_HEAD& 0x00FF))
        {
             SetIapDeviceBusyState();
            if(DoesAlsoGateDevice() == X_True)
            {
                sLoraDataCommon *p_common = (sLoraDataCommon *)p_data;
                
                temp_length = TwoBytesReverse(p_common ->head.length);
                if(temp_length == 0 || temp_length > MAX_LORA_FRAME_SIZE)
                {
                    SysLogDebug(1,("lora frame length %d error \r\n",temp_length));
                    return X_True;
                }
                temp_length -= 1;
                
                byteBufInit(temp_buf,MAX_LORA_FRAME_SIZE + (sizeof(sCommonResponseExt)/sizeof(uint8_t)),'\0');
                CopyBuffer(p_common ->payload,temp_buf,temp_length);
                ReportLogToClould((char *)temp_buf);
            }
            else
            {
                SysLogDebug(1,("sth wrong , the dest of log report is relay device ? \r\n"));
            }
            return X_True;
        }
        return X_False;
    }
    
    
#else // master device 
    X_Boolean IapCommandHandle(uint8_t * p_data,uint16_t length,uint32_t local_address)
    {
         uint16_t temp_length;
         sLoraDataHeader * p_header = (sLoraDataHeader *)p_data;
         uint8_t  temp_buf[MAX_LORA_FRAME_SIZE + 3];
        
        if(length < (LORA_DATA_HEADER_SIZE + 2) || length > MAX_LORA_RECV_DATA_LENGTH) {return X_False;}
        if(p_data == (uint8_t *)0) {return X_False;}
        
        if(p_header ->type == FRAME_TYPE_PROGRAM_CERTAIN_FILE)
        {
            if(ProgramCertainFileHandle(p_data,length) != X_True)
            {
                SysLogDebug(1,(" fatal error when get cmd : program certain file \r\n"));
            }
            return X_True;
        }
        else if(p_header ->type == FRAME_TYPE_FILES_VERSION_REQUEST
            && p_header ->header[0] == (LORA_CMD_FRAME_HEAD >> 8) && p_header ->header[1] == (uint8_t)(LORA_CMD_FRAME_HEAD& 0x00FF))
        {
            temp_length = GenerateLoraResponseData_AboutFilesVersion(&temp_buf[3],p_data);
           
            if(temp_length == 0 || temp_length > MAX_LORA_FRAME_SIZE)
            {
                SysLogDebug(1,(" fatal length error %d when report FILES_VERSION \r\n",temp_length));
            }

            temp_buf[0] = local_address >> 8;
            temp_buf[1] = (uint8_t)local_address;
            temp_buf[2] = GetRelativeLoraChannel(X_True);
            NearLora_SendRequest(temp_buf,temp_length + 3); 
            return X_True;
        }
        return X_False;
    }
    
    X_Boolean Does_AnIapDeviceBusyInTheLink(X_Void)
    {
        return X_False;
    }
    
    X_Void ActiveReportFilesVersion(X_Void)
    {
        uint8_t  temp_buf[MAX_LORA_FRAME_SIZE + 3];
        sLoraDataHeader fake_cmd_head;
        uint16_t temp_length;
        uint32_t local_address = TerminalNumberGet();
        u_AddrResolution local;
        
        local.address = local_address;
        local.bit.line_num = 0;
        fake_cmd_head.source_addr  = local.address;
        temp_length = GenerateLoraResponseData_AboutFilesVersion(&temp_buf[3],(uint8_t *)&fake_cmd_head);
           
        if(temp_length == 0 || temp_length > MAX_LORA_FRAME_SIZE)
        {
            SysLogDebug(1,(" fatal length error %d when report FILES_VERSION \r\n",temp_length));
        }

        temp_buf[0] = local_address >> 8;
        temp_buf[1] = (uint8_t)local_address;
        temp_buf[2] = GetRelativeLoraChannel(X_True);
        NearLora_SendRequest(temp_buf,temp_length + 3); 
    }
#endif
