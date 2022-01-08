#include "mTestMode.h"
#include "../../common/AppCommon.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define MAX_FRAME_LENGTH         (MAX_TEST_MODE_COMMAND_LENGTH + TEST_MODE_COMMAND_STRING_EXTERN_LENGTH + TEST_MODE_RECV_EXTERN_LENGTH)
#define MAX_FRAME_CHCHE_NUM      MAX_TM_COMMAND_CACHE_NUM 

#define TM_COMMAND_END_FLAG0            0x20  //
#define TM_COMMAND_END_FLAG1            0x4F  //O
#define TM_COMMAND_END_FLAG2            0x56	//V
#define TM_COMMAND_END_FLAG3            0x45	//E
#define TM_COMMAND_END_FLAG4            0x52	//R
/******************************************************************************/
typedef enum{
	FOP_idle,
	FOP_successed,
	FOP_errordata,
	FOP_inprocess,
	FOP_nospace,
}e_find_other_process;
/****************************************************************************************/
static X_Boolean DoesCommandThatNotBelongTestRoutine(uint8_t *p_command,uint16_t compare_length)
{
   if(memcmp(p_command,"ENTER",5) == 0) {return X_True;}
	 if(memcmp(p_command,"EXIT",4) == 0) {return X_True;}
	 return X_False;
}
/**********************************************************************************************************************************
0
*********************************************************************************************************************************/
StateNumber TM_IdleAction(s_StateMachineParam *p_this)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	p_ext ->recovery            = (tm_recovery_method)0;
	p_ext ->onWait              = (tm_onWait)0;
	p_ext ->onError             = (tm_onError)0;
	p_ext ->wait_counter        = 0;
	p_ext ->routine_cnt         = 0;
	p_ext ->failed_cnt          = 0;
	p_ext ->successed_cnt       = 0;
	p_ext ->routine_cnt         = 0;
	p_ext ->dot_cnt             = 0;
	p_ext ->isInSpecialMode     = X_False;
	p_ext ->isDotPrintDisable   = X_False;
	return TM_CommandAnalysis;
}
/**********************************************************************************************************************************
1
*********************************************************************************************************************************/
StateNumber TM_CommandAnalysisAction(s_StateMachineParam *p_this)
{
	uint8_t i,real_compare_length;
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	
	if(p_ext ->isNewCommandCome == X_True)
	{
		for(i=0;i<p_ext -> command_table_size;i++)
		{
			real_compare_length = 
			(p_ext ->temp_command_length > p_ext ->p_command_table[i].compare_length)? p_ext ->p_command_table[i].compare_length : p_ext ->temp_command_length;
			if((p_ext ->temp_command_length == p_ext ->p_command_table[i].length) &&
						(memcmp(p_ext ->temp_command_buf, p_ext ->p_command_table[i].command_string,real_compare_length) == 0))
			{
				
				if(p_ext ->p_command_table[i].isSpecialModeOnly == X_True)
				{
					if(p_ext ->isInSpecialMode == X_False)
					{
						TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method," please enter test mode first!!!\r\n"));
						p_ext ->isNewCommandCome = X_False;
						break;
					}
					else
					{
						p_ext ->test_mode_hold_cnt = p_ext ->test_mode_hold_time_in_ms;
					}
				}
				
				if(p_ext ->p_command_table[i].todo != X_Null) 
				{
					if(DoesCommandThatNotBelongTestRoutine(p_ext ->temp_command_buf,real_compare_length) == X_False)
					{
						TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"\r\n ( Begin Test Rountine ) \r\n"));
					}
					p_ext ->failed_cnt = 0;
					p_ext ->successed_cnt = 0;
					p_ext ->recovery = (tm_recovery_method)0;
					p_ext ->onWait   = (tm_onWait)0;
					p_ext ->onError  = (tm_onError)0;
					return p_ext ->p_command_table[i].todo(p_this);
				}
				p_ext ->isNewCommandCome = X_False;
				return p_this->current_state;
			}
		}
		
		if(i >= p_ext -> command_table_size) 
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method," Unrecognized TestMode Command !!!\r\n"));
			p_ext ->isNewCommandCome = X_False;
		}
		return p_this->current_state;	 
	}

	return p_this->current_state;	 
}
/**********************************************************************************************************************************
2
*********************************************************************************************************************************/
StateNumber TM_DoCommandRoutineAction(s_StateMachineParam *p_this)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	uint8_t index = (p_ext ->routine_cnt >= 1) ? (p_ext ->routine_cnt - 1)  : 0 ;
	
	if(p_ext ->routine_cnt >= 1) {p_ext ->routine_cnt --;}
	
	if(index < p_ext ->command_table_size)
	{
		if(p_ext ->p_command_table[index].todo != X_Null) 
		{
			return p_ext ->p_command_table[index].todo(p_this);
		}
		return p_this ->current_state;
	}
	return p_this ->current_state;
}
/**********************************************************************************************************************************
3
*********************************************************************************************************************************/
StateNumber TM_WaitAction(s_StateMachineParam *p_this)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	if(p_ext ->isDotPrintDisable  == X_False)
	{
		if(p_ext ->dot_cnt <= 1000)
		{
			p_ext ->dot_cnt += p_ext ->handle_called_freq_in_ms;
		}
		else
		{
			p_ext ->dot_cnt = 0;
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method," .\r"));
		}
	}
	
	if(p_ext ->onWait != X_Null)
	{
		if(p_ext ->onWait() == X_True) 
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"\r\n ---------[ OK ] \r\n"));
			p_ext ->successed_cnt ++;
			if(p_ext ->recovery != X_Null) {p_ext ->recovery();}
			return TM_Finish;
		}
	}
	if(p_ext ->onError != X_Null)
	{
		if(p_ext ->onError() == X_True)
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"\r\n --------- [ PROCESS Failed ]\r\n"));
			p_ext ->failed_cnt ++;
			if(p_ext ->recovery != X_Null) {p_ext ->recovery();}
			return TM_Finish;
		}
	}
	
	if((p_ext ->wait_counter < p_ext ->handle_called_freq_in_ms) )
	{
		if(p_ext ->onWait != X_Null)
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"\r\n --------- [ PROCESS TIME OUT ]\r\n"));
			p_ext ->failed_cnt ++;
		}
		else
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"\r\n --------- [ PROCESS NORMAL END ]\r\n"));
			p_ext ->successed_cnt ++;
		}
		
		return TM_Finish;
	}
	p_ext ->wait_counter -= p_ext ->handle_called_freq_in_ms;
	return p_this->current_state;
}
/**********************************************************************************************************************************
4
*********************************************************************************************************************************/
StateNumber TM_FinishAction(s_StateMachineParam *p_this)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	if(p_ext ->recovery != X_Null) {p_ext ->recovery();}
	if(p_ext ->routine_cnt > 0)
	{
		return TM_DoCommandRoutine;
	}
	else
	{
		p_ext ->isNewCommandCome = X_False;
		p_ext ->isDotPrintDisable = X_False;
		TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,
								"\r\n ( Test Rountine Finished ): \r\n[ %d PASSED ] \r\n[ %d FAILED ] \r\n"
								,p_ext ->successed_cnt,p_ext ->failed_cnt));
	}
	return TM_CommandAnalysis;
}
/**********************************************************************************************************************************
5
*********************************************************************************************************************************/
StateNumber TM_SpecialCmdFinishAction(s_StateMachineParam *p_this)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_this;
	p_ext ->isNewCommandCome = X_False;
	return TM_CommandAnalysis;
}
/**********************************************************************************************************************************
*********************************************************************************************************************************/
static X_Void StateJumpRecorder(StateNumber state_going_to_leave,StateNumber state_going_to_enter){}
static X_Void StopAllProcessInit(s_StateMachineParam *p_this)
{
	// no need null pointer check ,because the only caller is me 
	sTestModeParamExtern *p_ext = (sTestModeParamExtern*)p_this;
	p_ext ->test_mode_hold_cnt = p_ext ->test_mode_hold_time_in_ms;
	
	p_ext ->recovery            = (tm_recovery_method)0;
	p_ext ->onWait              = (tm_onWait)0;
	p_ext ->onError             = (tm_onError)0;
	p_ext ->wait_counter        = 0;
	p_ext ->routine_cnt         = 0;
	p_ext ->failed_cnt          = 0;
	p_ext ->successed_cnt       = 0;
	p_ext ->routine_cnt         = 0;
	p_ext ->dot_cnt             = 0;
//////////	p_ext ->isInSpecialMode     = X_False;  // no need
	p_ext ->isDotPrintDisable   = X_False;
	p_ext ->isNewCommandCome    = X_False;
	
	if(p_ext ->kill_all_method != X_Null) {p_ext ->kill_all_method();}
}
static X_Boolean TM_find_header(sTestModeRecvParam *p_recv,uint8_t current_data)
{
	//no need p_recv pointer check ,it is guarantee by the module
	if(p_recv ->header_index == 0 && current_data == 'T') {p_recv ->header_index = 1;return X_False;}
	if(p_recv ->header_index == 1 && current_data == 'M')
	{
		p_recv ->header_index = 0;
//////////		INSERT(LogDebug)(TEST_MODE_DATA_REC_DEBUG,(" TM command data get header \r\n"));
		return X_True;
	}
	p_recv ->header_index = 0;
	return X_False;
}
static X_Void TM_find_others(sTestModeRecvParam *p_recv,uint8_t current_data)
{
	uint16_t only_cmd_payload_length ;

	p_recv ->data_index ++;
	if(p_recv ->data_index >= p_recv ->max_recv_length)
	{
		p_recv ->header_index = 0;
		p_recv ->data_index   = 0;
		TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_recv ->send_buf_method,
															" TM command data receive error occur , reset to idle receive state!!!\r\n"));
		p_recv ->temp_rec_buf[0] = (uint8_t)FOP_idle;
		return;
	}
	if(p_recv ->temp_rec_buf[0] == FOP_idle)
	{
		//no need rec_buf pointer check ,it is guarantee by the module
		p_recv ->temp_rec_buf[TEST_MODE_RECV_EXTERN_LENGTH] = 'T';
		p_recv ->temp_rec_buf[TEST_MODE_RECV_EXTERN_LENGTH + 1] = 'M';
		p_recv ->data_index   = TEST_MODE_RECV_EXTERN_LENGTH + 2;
		p_recv ->temp_rec_buf[p_recv ->data_index] = current_data;
		p_recv ->temp_rec_buf[0] = (uint8_t)FOP_inprocess;
		return ;
	}

	if(p_recv ->temp_rec_buf[0] == FOP_inprocess)
	{
		p_recv ->temp_rec_buf[p_recv ->data_index] = current_data;

		if(p_recv ->data_index >= (TEST_MODE_COMMAND_STRING_EXTERN_LENGTH + TEST_MODE_RECV_EXTERN_LENGTH - 1) 
				&& current_data == TM_COMMAND_END_FLAG4 
				&& p_recv ->temp_rec_buf[p_recv ->data_index - 1] == TM_COMMAND_END_FLAG3
				&& p_recv ->temp_rec_buf[p_recv ->data_index - 2] == TM_COMMAND_END_FLAG2 
				&& p_recv ->temp_rec_buf[p_recv ->data_index - 3] == TM_COMMAND_END_FLAG1
				&& p_recv ->temp_rec_buf[p_recv ->data_index - 4] == TM_COMMAND_END_FLAG0)
		{
			//there is no whole length section in data , 
			//so I use two byte to store it
			only_cmd_payload_length = p_recv ->data_index + 1 - TEST_MODE_RECV_EXTERN_LENGTH - TEST_MODE_COMMAND_STRING_EXTERN_LENGTH;//p_recv ->data_index + 1 - TEST_MODE_RECV_EXTERN_LENGTH - TEST_MODE_COMMAND_STRING_EXTERN_LENGTH
			CopyBuffer(&only_cmd_payload_length,&p_recv ->temp_rec_buf[1],2);
			
			p_recv -> header_index = 0;
			p_recv ->temp_rec_buf[0] = (uint8_t)FOP_successed;
			return;
		}
		p_recv ->temp_rec_buf[0] = (uint8_t)FOP_inprocess;
		return;
	}
	p_recv -> header_index = 0;
	p_recv ->temp_rec_buf[0] = (uint8_t)FOP_idle;
	return;
}
static X_Boolean tm_ProtocolRecvInit(sTestModeRecvParam *p_manager)
{
	if(p_manager == X_Null) {return X_False;}
	if(p_manager ->temp_rec_buf == X_Null) {return X_False;} 
	
	if(p_manager ->max_recv_length == 0 || p_manager ->max_recv_length > MAX_FRAME_LENGTH) {return X_False;}
	if(p_manager ->max_command_length == 0 || p_manager ->max_command_length > MAX_TEST_MODE_COMMAND_LENGTH) {return X_False;}
	if(p_manager ->max_cache_num == 0 || p_manager ->max_cache_num > MAX_FRAME_CHCHE_NUM) {return X_False;}
	
	p_manager ->data_index   = 0;
	p_manager ->header_index = 0;
	p_manager ->cur_push_num = 0;
	p_manager ->cur_pop_num  = INVALID_LOOP_QUEUE_NODE_NUM;
	p_manager ->isHeaderFind = X_False;
	p_manager ->temp_rec_buf[0] = (uint8_t)FOP_idle;
	LoopQueueInitialize(p_manager ->p_queue);
	return X_True;
}
static X_Void tm_ProtocolRecvProcess(sTestModeRecvParam *p_manager,uint8_t data)
{
	uint16_t index,length;
	sTestModeParamExtern *p_ext = (sTestModeParamExtern*)p_manager ->p_state_param;
	if(p_manager == X_Null) {return;}

	if(p_manager ->isHeaderFind == X_False)
	{
		if(TM_find_header(p_manager,data) == X_True)
		{
			p_manager ->isHeaderFind = X_True;
			p_manager ->temp_rec_buf[0] = (uint8_t)FOP_idle;
		}
	}
	else
	{	
		TM_find_others(p_manager,data);
		
		if(p_manager ->temp_rec_buf[0] != FOP_inprocess)
		{
			p_manager ->isHeaderFind = X_False;
			if(p_manager ->temp_rec_buf[0] == FOP_successed)
			{
				if(memcmp(&p_manager ->temp_rec_buf[TEST_MODE_RECV_EXTERN_LENGTH + 2],"STOP",4) == 0)
				{
					ClearLoopQueue(p_manager ->p_queue);
					StopAllProcessInit(p_manager ->p_state_param);
					mStateMachineStateSet(p_manager ->p_state_machine,TM_CommandAnalysis);
					
					TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_manager ->send_buf_method,"@ test mode kill all process \r\n"));
					return;
				}
				
				index = GetLoopQueueUsedNodeNumber(p_manager ->p_queue);
				if(index > 0 || p_ext ->isNewCommandCome == X_True)
				{
					TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_manager ->send_buf_method,
					"\r\n ------- the latest command still in process , please wait or use command :TMSTOP to kill it \r\n"));
				}
				
				p_manager ->cur_push_num = LoopQueueFirstIn(p_manager ->p_queue,X_False);
				if(p_manager ->cur_push_num < p_manager ->max_cache_num)
				{
					index = p_manager ->cur_push_num*p_manager ->max_command_length;
					CopyBuffer(&p_manager ->temp_rec_buf[1],&length,2);
					if(length > p_manager ->max_command_length) 
					{
						RealseLoopQueueBuf(p_manager ->p_queue,p_manager ->cur_push_num);
						return;
					}
					CopyBuffer(&p_manager ->temp_rec_buf[1],&(p_manager ->p_rec_buf[index]),2);// + 2 for "TM"
					CopyBuffer(&p_manager ->temp_rec_buf[TEST_MODE_RECV_EXTERN_LENGTH + 2],&(p_manager ->p_rec_buf[index+2]),length);// + 2 for "TM",+2 for length
				}
			}
		}
	}
	
}
static X_Boolean tm_ProtocolRecvGetCommand(sTestModeRecvParam *p_manager,uint8_t *p_command,uint16_t *p_length)
{
	uint16_t index;

	if(p_manager == X_Null) {return X_False;}
	if(p_command == X_Null || p_length == X_Null)  {return X_False;}
	
	if(DoesLoopQueueEmpty(p_manager ->p_queue) == X_True){return X_False;}
	
	p_manager ->cur_pop_num = LoopQueueFirstOut(p_manager ->p_queue);
	if(p_manager ->cur_pop_num >= p_manager ->max_cache_num) {return X_False;}

	index = p_manager ->cur_pop_num*p_manager ->max_command_length;
	CopyBuffer(&p_manager ->p_rec_buf[index],p_length,2);
	
	if(*p_length > p_manager ->max_command_length)
	{
		RealseLoopQueueBuf(p_manager ->p_queue,p_manager ->cur_pop_num);
		return X_False;
	}
	CopyBuffer(&p_manager ->p_rec_buf[index + 2],p_command,*p_length);
	
	RealseLoopQueueBuf(p_manager ->p_queue,p_manager ->cur_pop_num);
	return X_True;
}
X_Void mTestModeInit(const sTestModeParam *p_tm)
{
	if(p_tm == X_Null) {return;}
	*p_tm ->p_isInitOK = X_False;
	if(tm_ProtocolRecvInit(p_tm ->p_recv) != X_True){return;}
	
	if(p_tm ->handle_called_freq_in_ms == 0){return;}
	if(p_tm ->get_byte_method == X_Null || p_tm ->send_buf_method == X_Null){return;}
	
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_tm ->p_state_param;
	if(p_ext ->handle_called_freq_in_ms == 0) {return;}
	if(p_ext ->get_byte_method == X_Null || p_ext ->send_buf_method == X_Null){return;}
	if(p_ext ->p_command_table == X_Null || p_ext ->command_table_size == 0) {return;}
	
	mStateMachineStateSet(p_tm ->p_state_machine,TM_Idle);
	
	*p_tm ->p_isInitOK = X_True;
}
X_Void mTestModeLoopReceive(const sTestModeParam *p_tm)
{
	uint8_t cur_data;
	if(p_tm == X_Null) {return;}
	if(*p_tm->p_isInitOK != X_True) {return;}
	
	if(p_tm ->get_byte_method(&cur_data) == X_True)
	{
		tm_ProtocolRecvProcess(p_tm ->p_recv,cur_data);
	}
}
X_Void mTestModeHandle(const sTestModeParam *p_tm)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern*)p_tm ->p_state_param;
	if(p_tm == X_Null) {return;}
	if(*p_tm->p_isInitOK != X_True) {return;}
	
	 if(p_ext ->isNewCommandCome != X_True)
	 {
			p_ext ->isNewCommandCome = tm_ProtocolRecvGetCommand(p_tm ->p_recv,p_ext ->temp_command_buf,&p_ext ->temp_command_length);
	 }
	
	mStateMachineRun(p_tm ->p_state_machine,p_tm->p_state_param,StateJumpRecorder);
	if(p_ext ->isInSpecialMode == X_True)
	{
		if(p_ext ->test_mode_hold_time_in_ms == TEST_SPECIAL_HOLD_TIME_INFINITE) {return;}
		if(p_ext ->test_mode_hold_cnt >= p_ext ->handle_called_freq_in_ms)
		{
			p_ext ->test_mode_hold_cnt -= p_ext ->handle_called_freq_in_ms;
		}
		else
		{
			TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"no command come in more than %d ms \r\n",p_ext ->test_mode_hold_time_in_ms));
			TM_EXIT_todo(p_tm ->p_state_param);
		}
	}
	
}
X_Boolean mTestModeDoesInSpecialMode(const sTestModeParam *p_tm)
{
	if(p_tm == X_Null) {return X_False;}
	sTestModeParamExtern *p_ext = (sTestModeParamExtern*)p_tm ->p_state_param;
	return  p_ext->isInSpecialMode;
}

X_Void mTM_SendString(tm_send_buf send_method,const char * sFormat, ...)
{
	uint16_t length,i;

		char g_pcOutBuf[MAX_TEST_MODE_OUTPUT_STRING_LENGTH];
		if(send_method == X_Null) {return;}
		for(i=0;i < MAX_TEST_MODE_OUTPUT_STRING_LENGTH;i++)
		{
			g_pcOutBuf[i] = '\0';
		}
    va_list args;
	
    va_start(args, sFormat);

		vsprintf(g_pcOutBuf,sFormat,args);
    va_end(args);
	
		length = strlen(g_pcOutBuf);
		if(length > MAX_TEST_MODE_OUTPUT_STRING_LENGTH) {length = MAX_TEST_MODE_OUTPUT_STRING_LENGTH;}
		send_method((uint8_t *)g_pcOutBuf,length);
}


static uint8_t GetRountineEventNum(sTestModeParamExtern *p_extern)
{
	uint8_t i,num;
	num = 0;
	for(i=0;i< p_extern ->command_table_size;i++)
	{
		num ++;
		if(memcmp(TEST_MODE_CMD_ROUTINE_END_STRING,p_extern ->p_command_table[i].command_string,p_extern ->p_command_table[i].compare_length) == 0)
		{
			break;
		}
	}
	
	if(i >= p_extern ->command_table_size) {return 0;}
	return num;
}
StateNumber TM_RUNALL_todo(s_StateMachineParam *p_base)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_base;
	p_ext ->routine_cnt = GetRountineEventNum(p_ext);
	return TM_DoCommandRoutine;
}
StateNumber TM_ENTER_todo(s_StateMachineParam *p_base)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_base;
	p_ext ->isInSpecialMode = X_True;
	
	p_ext ->test_mode_hold_cnt = p_ext ->test_mode_hold_time_in_ms;
	if(p_ext ->enter_method!= X_Null) {p_ext ->enter_method();}
	TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"@ enter test mode !\r\n"));
	return TM_SpecialCmdFinish;
}
StateNumber TM_EXIT_todo(s_StateMachineParam *p_base)
{
	sTestModeParamExtern *p_ext = (sTestModeParamExtern *)p_base;
	p_ext ->isInSpecialMode = X_False;
	if(p_ext ->exit_method!= X_Null) {p_ext ->exit_method();}
	TmLogPrintf(TEST_MODE_BASIC_LOG_DEBUG,(p_ext ->send_buf_method,"@ exit test mode !!!\r\n"));
	return TM_SpecialCmdFinish;
}





