#include "mFunc_ETH_Interface.h"
#include "mFunc_SoftTimer.h"
#include "mDri_Uart.h"
#include "../../../../../../module/customize/ProtocolRecv/ProtocolRecv.h"
#include "../../../../../../module/common/AppCommon.h"
#include "../../../../../../module/common/loopqueue/loop_queues.h"
////////////#include "ConsoleProtocol.h"
#include <string.h>

#define CHECK_SUM_SIZE_IN_BYTE     2
#define LENGTH_SIZE_IN_BYTE        2
#define WIFI_RECV_HEADER_SIZE_IN_BYTE        5
#define MIN_ETH_COMMAND_FRAME_LENGTH       6
#define MESSAGE_TYPE_SIZE_IN_BYTE      1
#define CONFIG_TYPE_SIZE_IN_BYTE        1
/******************************************************************************/
#define MAX_ETH_LOG_SEND_LENGTH   	200 // fix it later
 
#define MAX_ETH_FRAME_LENGTH   256 // fix it later :should + 11 in addition, because the wifi module will add another 11 byte head :D A 2B 49 50 44 2C 30 2C 39 3A
#define MAX_FRAME_TO_CACHE     2
#if ((MAX_ETH_FRAME_LENGTH > MAX_FRAME_LENGTH) || (MAX_FRAME_TO_CACHE > MAX_FRAME_CHCHE_NUM))
	#error [FRAME_LENGTH ,FRAME_TO_CACHE wrong !!!]
#endif
/******************************************************************************/
#define VALID_DATA_START_ADDR  3
/******************************************************************************/
static X_Boolean isSpecifiedReceive = X_False, isResponseGet = X_False; // isHugeBufNeedSend = X_False
static char special_rec_buf[MAX_ETH_RECEIVE_CHAR_LENGTH];
static uint16_t index = 0,current_length = 0;
static uint8_t special_cur_byte_backup = 0;

static uint8_t current_byte = 0;
static uint8_t header_index = 0,temp_index = 0;
static uint16_t whole_length = 0,whole_length_cnt = 0;
static X_Boolean isRecvLengthGet = X_False;

static X_DATA_UNIT eth_unit_receive(X_Void)
{
	return current_byte;
}

static X_Boolean eth_find_header(X_DATA_UNIT current_data)
{
////////	INSERT(LogDebug)(1,(" eth_find_header  \r\n",current_data));
	if(header_index == 0 && current_data == 0x2b) {header_index = 1;return X_False;}
	if(header_index == 1 && current_data == 0x49) {header_index = 2;return X_False;}
	if(header_index == 2 && current_data == 0x50) {header_index = 3;return X_False;}
	if(header_index == 3 && current_data == 0x44) {header_index = 4;return X_False;}

	if(header_index == 4 && current_data == 0x2c)
	{
		header_index = 0;
		isRecvLengthGet = X_False;
		INSERT(LogDebug)(ETH_DATA_DEBUG & ETH_DATA_RECV_DEBUG,(" eth command data get header \r\n"));
		return X_True;
	}
	header_index = 0;
	return X_False;
}


#include "bsp_UserLib.h"
static e_find_other_process eth_find_others(X_DATA_UNIT current_data,e_find_other_process const *p_fop,X_DATA_UNIT *p_buf)
{
	temp_index ++;
	if(temp_index > MAX_ETH_FRAME_LENGTH)
	{
		header_index = 0;
		temp_index   = 0;
		INSERT(LogDebug)(ETH_DATA_DEBUG | ETH_DATA_RECV_DEBUG,(" eth command data receive error occur , reset to idle!!!\r\n"));
		return FOP_idle;
	}
	if(*p_fop == FOP_idle)
	{
		//no need p_buf pointer check ,it is guarantee by the module
		p_buf[VALID_DATA_START_ADDR] = 0x2b;
		p_buf[VALID_DATA_START_ADDR + 1] = 0x49;
		p_buf[VALID_DATA_START_ADDR + 2] = 0x50;
		p_buf[VALID_DATA_START_ADDR + 3] = 0x44;
		p_buf[VALID_DATA_START_ADDR + 4] = 0x2c;
		
		temp_index   = VALID_DATA_START_ADDR + WIFI_RECV_HEADER_SIZE_IN_BYTE;
		whole_length = MAX_ETH_FRAME_LENGTH - 20;
		whole_length_cnt = whole_length;
		p_buf[temp_index] = current_data;// n 
		return FOP_inprocess;
	}

	if(*p_fop == FOP_inprocess)
	{
		p_buf[temp_index] = current_data;
		
		if(isRecvLengthGet == X_False)
		{
			if(current_data == 0x3a)
			{
				isRecvLengthGet = X_True;
				whole_length = StrToIntFix((char*)&p_buf[VALID_DATA_START_ADDR + WIFI_RECV_HEADER_SIZE_IN_BYTE]
																	,temp_index + 1 - VALID_DATA_START_ADDR - WIFI_RECV_HEADER_SIZE_IN_BYTE);
				whole_length_cnt = whole_length;
				INSERT(LogDebug)(ETH_DATA_DEBUG & ETH_DATA_RECV_DEBUG,(" eth command data get whole length = %d\r\n",whole_length));
			}
			
			return FOP_inprocess;
		}
		else
		{
			if(whole_length_cnt > 0 ) {whole_length_cnt --;}
		}

		if(whole_length_cnt == 0) 
		{
			INSERT(LogDebug)(ETH_DATA_DEBUG & ETH_DATA_RECV_DEBUG,(" eth command data get whole frame \r\n"));
			//there is no whole length section in data , 
			//so I use the first two byte to store it
			//the p_buf[1] index 1 is come from :
			//the protocol receive module use p_buf[0] to store the receive state , so that user data start from p_buf[1]
			CopyBuffer(&whole_length,&p_buf[1],LENGTH_SIZE_IN_BYTE);
			INSERT(LogDebug)(ETH_DATA_DEBUG | ETH_DATA_RECV_DEBUG,(" eth command data receive complete \r\n"));
			header_index = 0;
			return FOP_successed;
		}
		return FOP_inprocess;
	}
	header_index = 0;
	return FOP_idle;
}

PROTOCOL_RECV_DATA_BUF_DEF(p_recv,MAX_ETH_FRAME_LENGTH,MAX_FRAME_TO_CACHE,eth_unit_receive,eth_find_header,eth_find_others);

X_Void mFun_ETH_Init(X_Void)
{
	current_byte = 0;
	header_index = 0;
	temp_index = 0;
	whole_length = 0;
	isSpecifiedReceive = X_False;
	isResponseGet = X_False;
////////////	isHugeBufNeedSend = X_False;
	index = 0;
	current_length = 0;
	
	ProtocolRecvInit(p_recv);
	INSERT(LogDebug)(ETH_DATA_DEBUG,(" ProtocolRecvInit is %s \r\n"
	,(DoesProtocolRecvInitOK(p_recv) == X_True)? " OK" : " fatal error !!!!!!!"));
	
}

X_Void mFun_ETH_LoopReceive(X_Void)
{
	if(mDri_WifiGetByteInterface(&current_byte) == X_True)
	{
		INSERT(LogDebug)(ETH_DATA_DEBUG & ETH_DATA_RECV_DEBUG,(" eth port data get  %2x\r\n",current_byte));
		ProtocolRecvProcess(p_recv);
	}
}

X_Boolean mFun_ETH_DoesNewCommandCome(uint8_t **pp_data,uint16_t *p_length)
{
	uint8_t length_length;
	uint8_t *p_frame;
	uint16_t length;
	if(pp_data == X_Null || p_length == X_Null) {return X_False;}
	if(ProtocolRecvGetFrame(p_recv,&p_frame) == X_True)
	{
		//INSERT(LogDebug)(ETH_DATA_DEBUG | ETH_DATA_RECV_DEBUG,("\r\n"));
		CopyBuffer(p_frame,&length,LENGTH_SIZE_IN_BYTE);
		
		if(length < 10) {length_length = 1;}
		else if (length < 100) {length_length = 2;}
		else {length_length = 3;}
		
		*pp_data = &p_frame[ WIFI_RECV_HEADER_SIZE_IN_BYTE + length_length + VALID_DATA_START_ADDR];
		*p_length = length;
		return X_True;
	}
	return X_False;
}

X_Boolean mFunc_ETH_GetByte(uint8_t *p_data)
{
	if(p_data == X_Null) {return X_False;}
	return mDri_WifiGetByteInterface(p_data);
}
X_Boolean mFun_ETH_SendBuf(const uint8_t *p_data,uint16_t length)
{
	if(p_data == X_Null || length == 0 || length > (HUGE_ETH_FRAME_SEND_LENGTH + 2)) {return X_False;}
	INSERT(LogDebug)(ETH_SENDBUF_DEBUG,(" !!!! mFun_ETH_SendBuf length %d\r\n",length));
	return mDri_WifiSentBufInterface(p_data,length);
}
X_Void mFun_ETH_ReceiveBufClear(X_Void)
{
	mDri_WifiReceiveBufClear();
}
/************************************************************

***************************************************************/
INSERT(LOG_ONCE_ENTRY_DEF)(p_once,100);
static uint8_t eth_send_buf[MAX_ETH_SEND_FRAME_TO_CACHE][NORMAL_MAX_ETH_FRAME_SEND_LENGTH + 2], request_failed_cnt = 0;
static uint16_t cur_node_num = 0;
static X_Boolean isRecvSendRequest = X_False;

#define HUGE_BUF_TO_SEND_BUF_NUM    (5)  // 4130 / 1024 = 5
#define MAX_REQUEST_FAILED_TIMES     (10)

static uint8_t *p_huge_data;
static uint16_t total_length = 0,huge_buf_index = 0;

APP_LOOPQUEUE_DEF(p_send,MAX_ETH_SEND_FRAME_TO_CACHE);

X_Void mFunc_ETH_SendRequestInit(X_Void)
{
	LoopQueueInitialize(p_send);
	cur_node_num = 0;
	isRecvSendRequest = X_False;
	total_length = 0;
	huge_buf_index = 0;
	request_failed_cnt = 0;
}
X_Void mFunc_ETH_SendRequestEnable(X_Void)
{
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,0,("~~~~~~~~~~------------------mFunc_ETH_SendRequest  Enable\r\n"));
	isRecvSendRequest = X_True;
}
X_Void mFunc_ETH_SendRequestDisable(X_Void)
{
	INSERT(LogDebugOnce)(ETH_MODULE_DEBUG,p_once,1,("~~~~~~~~~~------------------mFunc_ETH_SendRequest  Disable !!!\r\n"));
	isRecvSendRequest = X_False;
}
X_Boolean mFun_ETH_SendBufRequest(const uint8_t *p_data,uint16_t length)
{
	uint16_t buf_num;
////////////	if(isRecvSendRequest == X_False) {return X_False;}
	if(p_data == X_Null || length == 0 || length > NORMAL_MAX_ETH_FRAME_SEND_LENGTH) {return X_False;}
	
	buf_num = LoopQueueFirstIn(p_send,X_False);
	if(buf_num < MAX_ETH_SEND_FRAME_TO_CACHE)
	{
		request_failed_cnt = 0;
		CopyBuffer(&length,&eth_send_buf[buf_num][0],2);
		CopyBuffer(p_data,&eth_send_buf[buf_num][2],length);
		INSERT(LogDebug)(ETH_TRANS_DEBUG,(" ------ eth get a send request : length %d\r\n",length));
		return X_True;
	}
	INSERT(LogDebug)(ETH_TRANS_DEBUG,(" ------ eth get a send request but no buf to store it !!!\r\n"));
	request_failed_cnt ++;
	if(request_failed_cnt >= MAX_REQUEST_FAILED_TIMES)
	{
		ClearLoopQueue(p_send);
	}
	return X_False;
}


X_Boolean mFun_ETH_SendHugeBufRequest(uint8_t *p_data,uint16_t length)
{
	if( p_data == X_Null || length == 0 || length > HUGE_BUF_TOTAL_SIZE) {return X_False;}
	INSERT(LogDebug)(ETH_TRANS_DEBUG,(" ------ eth get a huge send request : length %d\r\n",length));
	p_huge_data = p_data;
	total_length = length;
	huge_buf_index = 0;
	return X_True;
}
X_Boolean mFun_ETH_DoesSend(uint8_t **pp_data,uint16_t *p_length)
{
	uint16_t length,buf_num;
	if(pp_data == X_Null || p_length == X_Null) {return X_False;}
	if(DoesLoopQueueEmpty(p_send) == X_True){return X_False;}
	buf_num = LoopQueueFirstOut(p_send);
	if(buf_num < MAX_ETH_SEND_FRAME_TO_CACHE)
	{
		cur_node_num = buf_num;
		*pp_data = &eth_send_buf[buf_num][2];
		CopyBuffer(&eth_send_buf[buf_num][0],&length,LENGTH_SIZE_IN_BYTE);
		INSERT(LogDebug)(ETH_TRANS_DEBUG,(" ~~~~~ eth get a send buf : length %d \r\n",length));
		INSERT(LogDebug)(ETH_TRANS_DEBUG && (length == 37 || length == 46),(" ~~~~~ %s \r\n",*pp_data));
		
		if(length == 0 || length > NORMAL_MAX_ETH_FRAME_SEND_LENGTH) 
		{
			RealseLoopQueueBuf(p_send,cur_node_num); 
			return X_False;
		}
		*p_length = length;
		return X_True;
	}
	return X_False;
}
X_Boolean mFun_ETH_DoesHugeBufSend(uint8_t **pp_data,uint16_t *p_length)
{
	if(pp_data == X_Null || p_huge_data == X_Null || p_length == X_Null) {return X_False;}

	if(huge_buf_index < total_length)
	{
		*pp_data = &p_huge_data[huge_buf_index];
		if((huge_buf_index + UART_WIFI_MAX_TX_LENGTH) < total_length)
		{
			*p_length = UART_WIFI_MAX_TX_LENGTH;
		}
		else
		{
			*p_length = total_length - huge_buf_index;
		}
		huge_buf_index += UART_WIFI_MAX_TX_LENGTH;
		return X_True;
	}
	return X_False;
}
X_Void mFun_ETH_ReleaseCurSendBuf(X_Void)
{
	RealseLoopQueueBuf(p_send,cur_node_num); 
}
X_Void mFunc_ETH_SendAT(char *_Cmd)
{
	uint16_t length;
	uint8_t	buf[] = "\r\n";
	
	length = strlen(_Cmd);
	if(length > UART_WIFI_MAX_TX_LENGTH) {return;}
	mFun_ETH_SendBuf((uint8_t *)_Cmd,length );
	mFun_ETH_SendBuf(buf, 2);
}
X_Void mFun_ETH_LogDebug(const char * sFormat, ...)
{
		uint16_t length,i;
		char g_pcOutBuf[MAX_ETH_LOG_SEND_LENGTH];
		for(i=0;i < MAX_ETH_LOG_SEND_LENGTH;i++)
		{
			g_pcOutBuf[i] = '\0';
		}
    va_list args;
	
    va_start(args, sFormat);
////    len = 
		vsprintf(g_pcOutBuf,sFormat,args);
    va_end(args);
	
		length = strlen(g_pcOutBuf);
		if(length > MAX_ETH_LOG_SEND_LENGTH) {length = MAX_ETH_LOG_SEND_LENGTH;}
		mFun_ETH_SendBufRequest((uint8_t *)g_pcOutBuf,length);
}

X_Void mFunc_ETH_SpecificReceiveProcess(X_Void)
{
	uint8_t cur_byte;
	if(isSpecifiedReceive == X_True)
	{
			if(mFunc_SoftTimerGetValue(STID_ETH_Recv) == 0) 
			{
				isSpecifiedReceive = X_False;
				return;
			}
			if(index >= MAX_ETH_RECEIVE_CHAR_LENGTH) {return;}
			if(mDri_WifiGetByteInterface(&cur_byte) == X_True)
			{
				// debug code begin 
			#if (ETH_BYTE_RECEIVE_DEBUG != 0 && USE_INSERT_DEBUG != 0 )
				uint8_t eth_test_buf[2] = {"?."};
				eth_test_buf[0] = cur_byte;
				TM_LogDebug(1,("-%s ",eth_test_buf)); // something wrong when I use insert log debug here , can not understand ; fix it later
			#endif
			// debug code end 
				
				if(index >= 1)
				{
					if(special_cur_byte_backup != special_rec_buf[index - 1]) 
					{
						INSERT(LogDebug)(ETH_RECEIVE_DEBUG ,(" backup %2x != expect[%d] %2x\r\n"
															,special_cur_byte_backup,index - 1,special_rec_buf[index - 1]));
						index = 0;
					}
				}
				if(special_rec_buf[index] == cur_byte)
				{
					INSERT(LogDebug)(ETH_RECEIVE_DEBUG && (index > 10),(" matched [%d]:%2x %2x \r\n",index,special_rec_buf[index],cur_byte));
					index ++;
					if(index == current_length) 
					{
						isResponseGet = X_True;
						isSpecifiedReceive = X_False;
						INSERT(LogDebug)(ETH_RECEIVE_DEBUG | ETH_SPECIAL_RECEIVE_DEBUG,(" total matched !! current_length = %d \r\n",current_length));
					}
				}
				special_cur_byte_backup = cur_byte;
			}
	}
}


X_Boolean mFunc_ETH_WaitResponseEventRegister(char *p_string_to_wait)
{
	uint16_t i;
	
	isResponseGet = X_False;
	index 				= 0;
	special_cur_byte_backup = 0;
	current_length = strlen(p_string_to_wait);
	if (current_length >= MAX_ETH_RECEIVE_CHAR_LENGTH){return X_False;}

	mDri_WifiReceiveBufClear();
	for(i=0;i<MAX_ETH_RECEIVE_CHAR_LENGTH;i++)
	{
		special_rec_buf[i] = '\0';
	}
	
	for(i=0;i<current_length;i++)
	{
		special_rec_buf[i] = p_string_to_wait[i];
	}
	INSERT(LogDebug)(ETH_RECEIVE_DEBUG | ETH_SPECIAL_RECEIVE_DEBUG,("ETH expect receive  %d byte string %s\r\n",current_length,special_rec_buf));
	isSpecifiedReceive = X_True;
	return X_True;
}


X_Boolean mFunc_ETH_DoesResponseGet(X_Void)
{
	return isResponseGet;
}

X_Boolean mFunc_ETH_DoesRecvSendRequest(X_Void)
{
	return isRecvSendRequest;
}


