#include "ProtocolRecv.h"
#define BAD_FRAME_FLAG 		0x77
#define GOOD_FRAME_FLAG 	0x99  // 

X_Boolean ProtocolRecvInit(const sProtocolRecv *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null){return X_False;}
	if(p_manager ->p_buf == X_Null) {return X_False;} 
	if(p_manager ->p_process == X_Null) {return X_False;} 
	if(p_manager ->func_recv == X_Null || p_manager ->func_fheader == X_Null || p_manager ->func_fothers == X_Null){return X_False;}
	if(p_manager ->max_frame_length == 0 || p_manager ->max_frame_length > MAX_FRAME_LENGTH) {return X_False;}
	if(p_manager ->max_frame_num == 0 || p_manager ->max_frame_num > MAX_FRAME_CHCHE_NUM) {return X_False;}
	*p_manager ->isInitOK = X_True;
	return X_True;
}
//#include <stdio.h>
X_Void ProtocolRecvProcess(const sProtocolRecv *p_manager)
{
	X_DATA_UNIT data;
	if(p_manager == X_Null) {return;}
	if(p_manager ->isInitOK == X_Null)  {return;}
	if(*p_manager->isInitOK == X_False) {return;}

	
	data =  p_manager ->func_recv();

	if(p_manager ->p_process->isHeaderFind == X_False)
	{
		if(p_manager ->func_fheader(data) == X_True)
		{
			p_manager ->p_process->isHeaderFind = X_True;
			p_manager ->p_process->process      = FOP_idle;
		}
	}
	else
	{
		if(p_manager ->func_fothers(data,&p_manager ->p_process->process) != FOP_inprocess){p_manager ->p_process->isHeaderFind = X_False;}
	}
	
}
X_Boolean ProtocolRecvGetFrame(const sProtocolRecv *p_manager,X_DATA_UNIT **pp_buf,uint16_t *p_length)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null)  {return X_False;}
	if(*p_manager->isInitOK == X_False) {return X_False;}

	UNUSED_PARAMETER(pp_buf);UNUSED_PARAMETER(p_length);
	
	return X_True;
}
X_Boolean DoesProtocolRecvInitOK(const sProtocolRecv *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null)  {return X_False;}
	return (*p_manager ->isInitOK);
}



