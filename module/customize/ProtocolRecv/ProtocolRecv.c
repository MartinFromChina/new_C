#include "ProtocolRecv.h" 
//#include <stdio.h>

X_Boolean ProtocolRecvInit(const sProtocolRecv *p_manager)
{
	uint16_t i;
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null){return X_False;}
	if(p_manager ->p_buf == X_Null) {return X_False;} 
	if(p_manager ->p_process == X_Null) {return X_False;} 
	if(p_manager ->p_operation == X_Null) {return X_False;} 
	if(p_manager ->func_recv == X_Null || p_manager ->func_fheader == X_Null || p_manager ->func_fothers == X_Null){return X_False;}
	if(p_manager ->max_frame_length == 0 || p_manager ->max_frame_length > MAX_FRAME_LENGTH) {return X_False;}
	if(p_manager ->max_frame_num == 0 || p_manager ->max_frame_num > MAX_FRAME_CHCHE_NUM) {return X_False;}
	
	p_manager ->p_operation ->cur_push_node = 0;
	p_manager ->p_operation ->cur_pop_node = INVALID_NODE_NUM;
	p_manager ->p_operation ->queue_init(p_manager ->p_operation ->p_manager);
	for(i = 0;i < p_manager->max_frame_num ; i++)
	{
		p_manager ->p_buf[i*p_manager ->max_frame_length] = BAD_FRAME_FLAG;
	}
	*p_manager ->isInitOK = X_True;
	return X_True;
}

X_Void ProtocolRecvProcess(const sProtocolRecv *p_manager)
{
	X_DATA_UNIT data;
	uint16_t index;
	if(p_manager == X_Null) {return;}
	if(p_manager ->isInitOK == X_Null)  {return;}
	if(*p_manager->isInitOK == X_False) {return;}

	
	data =  p_manager ->func_recv();

	if(p_manager ->p_process->isHeaderFind == X_False)
	{
		if(p_manager ->func_fheader(data) == X_True)
		{
			p_manager ->p_operation ->cur_push_node = p_manager ->p_operation ->queue_fi(p_manager ->p_operation->p_manager,X_False);
			if(p_manager ->p_operation ->cur_push_node < p_manager ->max_frame_num)
			{
				p_manager ->p_process->isHeaderFind = X_True;
				p_manager ->p_process->process      = FOP_idle;
				index = p_manager ->p_operation ->cur_push_node*p_manager ->max_frame_length;
				p_manager ->p_buf[index] = RECV_PROCESS_FLAG;
			}
		}
	}
	else
	{	
		if(p_manager ->p_operation ->cur_push_node >= p_manager ->max_frame_num) 
		{
			//should never come here
			p_manager ->p_process->isHeaderFind = X_False;
			p_manager ->p_operation ->queue_clear(p_manager ->p_operation ->p_manager);
			return;
		}
		index = p_manager ->p_operation ->cur_push_node*p_manager ->max_frame_length;
		p_manager ->p_process ->process = p_manager ->func_fothers(data,&p_manager ->p_process->process,&(p_manager ->p_buf[index]));
		
		if(p_manager ->p_process ->process!= FOP_inprocess)
		{
			p_manager ->p_process->isHeaderFind = X_False;
		}
	}
	
}
X_Boolean ProtocolRecvGetFrame(const sProtocolRecv *p_manager,X_DATA_UNIT **pp_buf)
{
	uint16_t index;
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null)  {return X_False;}
	if(*p_manager->isInitOK == X_False) {return X_False;}
	if( pp_buf == X_Null) {return X_False;}

	if(p_manager ->p_operation ->cur_pop_node < p_manager ->max_frame_num) // maybe push in process not finished yet
	{
		index = p_manager ->p_operation ->cur_pop_node*p_manager ->max_frame_length;
		if(p_manager ->p_buf[index] == RECV_PROCESS_FLAG) 
		{
			return X_False;
		}
		else if(p_manager ->p_buf[index] != GOOD_FRAME_FLAG)
		{
			//printf("-----bad frame ,drop it \r\n");
			p_manager ->p_operation ->queue_release(p_manager ->p_operation ->p_manager,p_manager ->p_operation ->cur_pop_node);
			p_manager ->p_operation ->cur_pop_node = INVALID_NODE_NUM;
			return X_False;
		}
		else 
		{
			*pp_buf = &p_manager ->p_buf[index + 1];	
			p_manager ->p_operation ->queue_release(p_manager ->p_operation ->p_manager,p_manager ->p_operation ->cur_pop_node);
			p_manager ->p_operation ->cur_pop_node = INVALID_NODE_NUM;
			p_manager ->p_buf[index] = BAD_FRAME_FLAG;
			return X_True;
		}
	}
	
	if(p_manager ->p_operation ->queue_empty( p_manager ->p_operation ->p_manager) == X_True){return X_False;}

	p_manager ->p_operation ->cur_pop_node = p_manager ->p_operation ->queue_fo( p_manager ->p_operation ->p_manager);
	
	if(p_manager ->p_operation ->cur_pop_node >= p_manager ->max_frame_num) {return X_False;}

	index = p_manager ->p_operation ->cur_pop_node*p_manager ->max_frame_length;
	if(p_manager ->p_buf[index] == RECV_PROCESS_FLAG) 
	{
		return X_False;
	}
	else if(p_manager ->p_buf[index] != GOOD_FRAME_FLAG)
	{
		//printf("------------------bad frame ,drop it \r\n");
		p_manager ->p_operation ->queue_release(p_manager ->p_operation ->p_manager,p_manager ->p_operation ->cur_pop_node);
		p_manager ->p_operation ->cur_pop_node = INVALID_NODE_NUM;
		return X_False;
	}
	else 
	{
		*pp_buf = &p_manager ->p_buf[index + 1];	
		p_manager ->p_operation ->queue_release(p_manager ->p_operation ->p_manager,p_manager ->p_operation ->cur_pop_node);
		p_manager ->p_operation ->cur_pop_node = INVALID_NODE_NUM;
		p_manager ->p_buf[index] = BAD_FRAME_FLAG;
		return X_True;
	}
}
X_Boolean DoesProtocolRecvInitOK(const sProtocolRecv *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->isInitOK == X_Null)  {return X_False;}
	return (*p_manager ->isInitOK);
}



