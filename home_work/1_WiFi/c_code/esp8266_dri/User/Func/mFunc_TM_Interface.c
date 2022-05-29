#include "mFunc_TM_Interface.h"
#include "mDri_Uart.h"
//////#include "../../../../common/module/customize/ProtocolRecv/ProtocolRecv.h"
#include "../../../../../../module/common/AppCommon.h"
/******************************************************************************/
#define MAX_TM_FRAME_LENGTH   		200
/******************************************************************************/



static X_Boolean isReceiveLocked = X_False;

X_Void mFun_TM_Init(X_Void)
{
	isReceiveLocked = X_False;
}
X_Void mFun_TM_ReceiveClear(X_Void)
{
	mDri_TestModeReceiveBufClear();
}
extern X_Boolean  isWifiPortRawData;
X_Boolean mFun_TM_GetByte(uint8_t *p_data)
{
	if(p_data == X_Null) {return X_False;}
	if(isReceiveLocked == X_True) {return X_False;}

	
	if(mDri_TestModeGetByteInterface(p_data) == X_True)
	{
		if(isWifiPortRawData == X_True)
		{
			mDri_WifiSentBufInterface(p_data,1);
		}
		return X_True;
	}
	return X_False;
}


X_Boolean mFun_TM_SendBuf(const uint8_t *p_data,uint16_t length)
{
	if(p_data == X_Null || length == 0 || length > MAX_TM_FRAME_SEND_LENGTH) {return X_False;}
	return mDri_TestModeSentBufInterface(p_data,length);
}
#include <string.h>
X_Void mFun_TM_LogDebug(const char * sFormat, ...)
{
	uint16_t length,i;
//    int len;
		char g_pcOutBuf[MAX_TM_FRAME_SEND_LENGTH];
		for(i=0;i < MAX_TM_FRAME_SEND_LENGTH;i++)
		{
			g_pcOutBuf[i] = '\0';
		}
    va_list args;
	
    va_start(args, sFormat);
////    len = 
		vsprintf(g_pcOutBuf,sFormat,args);
    va_end(args);
	
		length = strlen(g_pcOutBuf);
		if(length > MAX_TM_FRAME_SEND_LENGTH) {length = MAX_TM_FRAME_SEND_LENGTH;}
		mDri_TestModeSentBufInterface((uint8_t *)g_pcOutBuf,length);
}
X_Void mFun_TM_ReceiveLocked(X_Void)
{
	isReceiveLocked = X_True;
}
X_Void mFun_TM_ReceiveUnocked(X_Void)
{
	isReceiveLocked = X_False;
}

