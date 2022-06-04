#include  "LogDebugSwitch.h"
#include  "SEGGER_RTT.h"

#if (USE_SEGGER_RTT_READ_COMMAND != 0)
static X_Void SeggerRttReadInit(X_Void);
#endif

X_Void LogDebugInit(X_Void)
{
	#if (USE_INSERT_DEBUG != 0)
	#if (USE_SEGGER_RTT_READ_COMMAND != 0)
		SeggerRttReadInit();
	#endif
	SEGGER_RTT_Init();
	#endif
}


X_Void SeggerRTT_LogDebug(const char * sFormat, ...)
{
	va_list ParamList;
  va_start(ParamList, sFormat);
  SEGGER_RTT_vprintf(0,sFormat, &ParamList);
  va_end(ParamList);
}

/**********************************************************************/
/******************************for test mode     **********************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
#if (USE_SEGGER_RTT_READ_COMMAND != 0)
#include "../../../../../../module/common/loopqueue/loop_queues.h"
#include "../../../../../../module/common/AppCommon.h"

#define MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES          50

APP_LOOPQUEUE_DEF(p_recv,MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES);

static uint8_t read_buf[MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES];
static X_Void SeggerRttReadInit(X_Void)
{
	uint16_t i;
	for(i=0;i<sizeof(read_buf)/sizeof(uint8_t);i++)
	{
		read_buf[i] = ' ';
	}
	LoopQueueInitialize(p_recv);
}

static uint8_t recv_buf[MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES];
static X_Void DebugByteLoad(uint8_t cur_byte)
{
	uint16_t buf_num;
	
	buf_num = LoopQueueFirstIn(p_recv,X_False);
	if(buf_num < MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES)
	{
		recv_buf[buf_num] = cur_byte;
	}
}
X_Void SeggerRttLoopRead(X_Void)
{
	uint16_t i;
	unsigned char bytes_num = 0;
	bytes_num = SEGGER_RTT_Read(0,read_buf,sizeof(read_buf)/sizeof(uint8_t));
	if(bytes_num > 0 && bytes_num <=MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES)
	{
		for(i=0;i< bytes_num;i++)
		{
			DebugByteLoad(read_buf[i]);
		}
	}
}

X_Void mDri_TestModeReceiveBufClear(X_Void)
{
	uint16_t i;
	for(i=0;i<sizeof(read_buf)/sizeof(uint8_t);i++)
	{
		read_buf[i] = ' ';
	}
	LoopQueueInitialize(p_recv);
}

X_Boolean mDri_TestModeGetByteInterface(uint8_t *p_data)
{
	uint16_t buf_num;
	if(p_data != X_Null /*&& isNewByteCome == X_True*/)
	{
		if(DoesLoopQueueEmpty(p_recv) == X_True) {return X_False;}
		buf_num = LoopQueueFirstOut(p_recv);
		if(buf_num < MAX_SEGGER_RTT_RECV_SIZE_IN_BYTES)
		{
			*p_data = recv_buf[buf_num];
			RealseLoopQueueBuf(p_recv,buf_num);
			INSERT(LogDebug)(TEST_MODE_DATA_REC_DEBUG,(" test mode get byte %2x\r\n",recv_buf[buf_num]));
			return X_True;
		}
		return X_False;
	}
	return X_False;
}

X_Boolean mDri_TestModeSentBufInterface(const uint8_t *p_buf,uint16_t length)
{
	if(p_buf == X_Null || length > MAX_TM_FRAME_SEND_LENGTH) {return X_False;}
	return (SEGGER_RTT_Write(0,p_buf,length) == length);
}

X_Boolean mDri_DoesTestModeSentBufEmpty(X_Void)
{
	return X_True;
}
#endif