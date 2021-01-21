#include "ProtocolRecv.h"

X_Boolean ProtocolRecvInit(const sProtocolRecv *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null || p_manager ->isInitCalled == X_Null){return X_False;}
	*p_manager ->isInitCalled = X_True;
	if(p_manager ->p_buf == X_Null) {return X_False;} 
	if(p_manager ->func_recv == X_Null || p_manager ->func_fheader == X_Null || p_manager ->func_fothers == X_Null){return X_False;}
	if(p_manager ->max_frame_length == 0 || p_manager ->max_frame_length > MAX_FARME_LENGTH) {return X_False;}
	if(p_manager ->max_frame_num == 0 || p_manager ->max_frame_num > MAX_FRAME_CHCHE_NUM) {return X_False;}
	*p_manager ->isInitOK = X_True;
}
X_Void ProtocolRecvProcess(const sProtocolRecv *p_manager)
{

}
X_Boolean ProtocolRecvGetFrame(const sProtocolRecv *p_manager,X_DATA_UNIT *p_buf,uint16_t length)
{
	return X_True;
}
X_Boolean DoesProtocolRecvInitOK(const sProtocolRecv *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null || p_manager ->isInitCalled == X_Null)  {return X_False;}
	if(*p_manager ->isInitCalled == X_False)  {return X_False;}
	return (*p_manager ->isInitOK);
}



