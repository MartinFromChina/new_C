#include "cp_drive.h"
#include "../../common/AppCommon.h"
/*
#include "../../common/AppCommon.h"
#include <stdio.h>
void just_test (void)
{
    uint32_t  i = 110;
    uint32_t  j = 1; 
    uint32_t  r = j - i;
    printf("result is %d   %2x  %u \r\n",r,r,r);  // 4294967187  4,294,967,295

    uint8_t  i1 = 110;
    uint8_t  j1 = 1; 
    uint8_t  r1 = j1 - i1;
    printf("result is %d   %2x \r\n",r1,r1);

    uint16_t  i2 = 110;
    uint16_t  j2 = 1; 
    uint16_t  r2 = j2 - i2;
    printf("result is %d   %2x \r\n",r2,r2);
}

*/

/************************************************************************************************/
#define SEND_PROCESS_CHECK                 0
#define SEND_PROCESSING                    1
#define SEND_PROCESS_WAIT_RESPONSE         2

static X_Void BeforeSendProcess(const s_cp_manager * p_manager)
{
    *p_manager ->p_is_send_again                    = 0;
    p_manager ->p_send_param ->result               = 0;
    p_manager ->p_send_param ->is_new_send_request  = 1;
}

static X_Void send_handle(const s_cp_manager * p_manager,uint32_t ms ,uint8_t is_new,uint8_t cur_data)
{
    // no need null pointer check , because the only caller is cp_dri_handle 
    uint16_t buf_num;
    /*~~~~~~~~~~~~~~~~~send ~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    if(p_manager ->send == (hsm_task_func)0) {return;}
    
    if(* p_manager ->p_is_send_processing == SEND_PROCESS_CHECK)
    {
        if(DoesLoopQueueEmpty(p_manager ->p_send_list) == X_True) {return;}
        
        buf_num = LoopQueueFirstOut(p_manager ->p_send_list);
        if(buf_num < p_manager ->send_cache_num)
        {
            if(p_manager ->p_send_addition[buf_num].send_length > p_manager ->max_send_length
                || p_manager ->p_send_addition[buf_num].send_length == 0)
            {
                RealseLoopQueueBuf(p_manager ->p_send_list,buf_num);
                ////return;
            }
            else
            {
                CopyBuffer(&p_manager ->p_send_buf[buf_num * p_manager ->max_send_length],
                        p_manager ->p_send_param->p_send_data,
                        p_manager ->p_send_addition[buf_num].send_length);
                p_manager ->p_send_param ->send_length       = p_manager ->p_send_addition[buf_num].send_length;

                p_manager ->p_cur_send->send_length          = p_manager ->p_send_addition[buf_num].send_length;
                p_manager ->p_cur_send->rec_time_limit_in_ms = p_manager ->p_send_addition[buf_num].rec_time_limit_in_ms;
                p_manager ->p_cur_send->send_cb              = p_manager ->p_send_addition[buf_num].send_cb;
                p_manager ->p_cur_send->recv_cb              = p_manager ->p_send_addition[buf_num].recv_cb;

                RealseLoopQueueBuf(p_manager ->p_send_list,buf_num);
                
                BeforeSendProcess(p_manager);
                * p_manager ->p_is_send_processing = SEND_PROCESSING; // enable send process
                ////return;
            }
        }
        return;
    }
    else  if(* p_manager ->p_is_send_processing == SEND_PROCESSING)// send process
    {
        p_manager ->p_send_param->is_new_data_come = is_new;
        p_manager ->p_send_param->new_data         = cur_data;
        
        if(p_manager ->send(ms,&p_manager ->p_send_param->base[CP_SEND_HSM_DEEPTH]) == X_False)
        {
            // take care the send result;
            if(p_manager ->p_cur_send->send_cb == (cp_send_rec_cb)0)
            {
                    // take care the wait response request ; 
                if(p_manager ->p_cur_send ->rec_time_limit_in_ms == 0 
                || p_manager ->p_cur_send ->recv_cb == (cp_send_rec_cb)0)
                {
                        * p_manager ->p_is_send_processing = SEND_PROCESS_CHECK;
                }
                else
                {
                    //
                    *p_manager ->p_wait_ms_backup    = ms;
                    *p_manager ->p_is_wait_response  = 1;
                    /////p_manager ->p_send_param->wait_result       = 0;
                    * p_manager ->p_is_send_processing          = SEND_PROCESS_WAIT_RESPONSE;
                }
                return;
            }
            
            if(p_manager ->p_send_param ->result == 0)
            {
                p_manager ->p_cur_send->send_cb(CSRE_Error,
                        p_manager ->p_send_param->p_send_data,p_manager ->p_cur_send->send_length,ms);
            }
            else
            {
                p_manager ->p_cur_send->send_cb(CSRE_Successed,
                        p_manager ->p_send_param->p_send_data,p_manager ->p_cur_send->send_length,ms);
            }

                // take care the wait response request ; 
            if(*p_manager ->p_is_send_again == 0)
            {
                if(p_manager ->p_send_param ->result == 0)
                {
                    * p_manager ->p_is_send_processing = SEND_PROCESS_CHECK;
                }
                else
                {
                    if(p_manager ->p_cur_send ->rec_time_limit_in_ms == 0 
                    || p_manager ->p_cur_send ->recv_cb == (cp_send_rec_cb)0)
                    {
                        * p_manager ->p_is_send_processing = SEND_PROCESS_CHECK;
                    }
                    else
                    {
                        *p_manager ->p_wait_ms_backup    = ms;
                        *p_manager ->p_is_wait_response  = 1;
                        * p_manager ->p_is_send_processing          = SEND_PROCESS_WAIT_RESPONSE;
                    }
                }
                
            }
            else
            {
                 BeforeSendProcess(p_manager);
                * p_manager ->p_is_send_processing = SEND_PROCESSING;
            }
        }
        return;
    }    
    else // must be SEND_PROCESS_WAIT_RESPONSE
    {
        if(*p_manager ->p_is_wait_response == 0)
        {
            * p_manager ->p_is_send_processing = SEND_PROCESS_CHECK;
        }
        else
        {
            if((ms - (*p_manager->p_wait_ms_backup) ) > p_manager ->p_cur_send ->rec_time_limit_in_ms) // time out 
            {
                if( *p_manager ->p_is_send_again  == 0)
                {
                    *p_manager ->p_is_wait_response = 0;

                    p_manager ->p_cur_send ->recv_cb(CSRE_Timeout,
                                        p_manager ->p_send_param->p_send_data,p_manager ->p_cur_send->send_length,ms);

                    if(*p_manager ->p_is_send_again == 0)
                    {
                        * p_manager ->p_is_send_processing = SEND_PROCESS_CHECK;
                    }
                    else
                    {
                        BeforeSendProcess(p_manager);
                        * p_manager ->p_is_send_processing = SEND_PROCESSING; // enable send process
                    }

                }
                else
                {
                    BeforeSendProcess(p_manager);
                    * p_manager ->p_is_send_processing = SEND_PROCESSING; // enable send process
                }
               
                ///// return;
            }
        }
        return;
    }
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}
static X_Void recv_handle(const s_cp_manager * p_manager, uint32_t ms , uint8_t is_new,uint8_t cur_data)
{
    // no need null pointer check , because the only caller is cp_dri_handle 
    uint16_t buf_num;
    /*~~~~~~~~~~~~~~~~~~~~~recv ~~~~~~~~~~~~~~~~~~~~~~*/
    if(p_manager ->recv != (hsm_task_func)0) 
    {
        p_manager ->p_recv_param->is_new_data_come = is_new;
        p_manager ->p_recv_param->new_data         = cur_data;

        if(p_manager ->recv(ms,&p_manager ->p_recv_param->base[CP_RECV_HSM_DEEPTH]) == X_True) {return;}    
        
        if(p_manager ->p_recv_param ->is_new_recv == 0 ||  p_manager ->p_recv_param ->recv_length > p_manager ->p_recv_param ->max_length) {return;}   

        p_manager ->p_recv_param ->is_new_recv = 0;
        if(*p_manager ->p_is_wait_response != 0)
        {
            if(p_manager ->p_cur_send ->recv_cb(CSRE_RecvCheck,
                            p_manager ->p_recv_param->p_buf,p_manager ->p_recv_param ->recv_length,ms) == X_True)
            {
                *p_manager ->p_is_wait_response = 0;

                p_manager ->p_cur_send ->recv_cb(CSRE_Successed,
                            p_manager ->p_recv_param->p_buf,p_manager ->p_recv_param ->recv_length,ms);
                return ; 
            }
        }

        buf_num = LoopQueueFirstIn(p_manager ->p_recv_list,X_False);
		if(buf_num >= p_manager ->recv_cache_num)
		{
			/////INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ----  lora GetMessage unexpected error occur 1\r\n"));
			// use event call back to report the buf full situation 
			return ;
		}
		
		CopyBuffer(&p_manager ->p_recv_param ->recv_length,
					&p_manager ->p_recv_buf[buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)],
					CP_LENGTH_SIZE_IN_BYTE);
        CopyBuffer(&ms,
					&p_manager ->p_recv_buf[(buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)) + CP_LENGTH_SIZE_IN_BYTE],
					CP_TIMESTAMP_SIZE_IN_BYTE);
		CopyBuffer(p_manager ->p_recv_param->p_buf,
					&p_manager ->p_recv_buf[(buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)) + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE],
					p_manager ->p_recv_param ->recv_length);
    }
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
}
/**********************************************************************
 **********************************************************************/

static X_Boolean hsm_param_init(s_T_taskBaseParam * p_top_param,uint8_t deepth)
{
    uint8_t i;
    s_T_taskBaseParam * p_cur = p_top_param;
    // no need null pointer check ,because the only caller is hsm self ;
    hsm_one_level_param_Init(p_top_param);

    for(i=0; i<deepth; i++)
    {
        if(p_cur -> p_level_next != (s_T_taskBaseParam *)0)
        {
            p_cur = p_cur ->p_level_next;
        }
        else
        {
            return X_False;
        }

        hsm_one_level_param_Init(p_cur);
    }

    return X_True;
}

static X_Boolean reset_param(const s_cp_manager * p_manager)
{
    // no need null pointer check , because the only caller is cp_dri_handle 
    *p_manager ->p_is_freeze  = 0;
    if(hsm_param_init(&p_manager ->p_event_param ->base[CP_EVENT_HSM_DEEPTH],CP_EVENT_HSM_DEEPTH) != X_True) {return X_False;}
    p_manager ->p_event_param->is_new_data_come = 0;
    p_manager ->p_event_param->new_data         = 0;

    if(hsm_param_init(&p_manager ->p_recv_param ->base[CP_RECV_HSM_DEEPTH],CP_RECV_HSM_DEEPTH) != X_True) {return X_False;}
    if(p_manager ->p_recv_param->max_length < (CP_LENGTH_SIZE_IN_BYTE + 1)) {return X_False;}
    p_manager ->p_recv_param ->recv_length      = 0;
    p_manager ->p_recv_param ->is_new_data_come = 0;
    p_manager ->p_recv_param ->new_data         = 0;
    p_manager ->p_recv_param ->is_new_recv      = 0;

    if(hsm_param_init(&p_manager ->p_send_param ->base[CP_SEND_HSM_DEEPTH],CP_SEND_HSM_DEEPTH) != X_True) {return X_False;}
    *p_manager ->p_is_send_processing           = 0;
    p_manager ->p_send_param ->result           = 0;
    *p_manager ->p_is_send_again                = 0;
    *p_manager ->p_is_wait_response             = 0;
    *p_manager ->p_wait_ms_backup               = 0;
    p_manager ->p_send_param ->is_new_data_come = 0;
    p_manager ->p_send_param ->new_data         = 0;

    *p_manager ->p_cur_event_index              = 0;
    ///////////if(p_manager ->max_send_length == 0)
    if(p_manager ->event_num > CP_MAX_EVENT_NUM)  {return X_False;}
    LoopQueueInitialize(p_manager ->p_send_list);
    LoopQueueInitialize(p_manager ->p_recv_list);

    *p_manager ->p_query_freq = p_manager ->query_freq_reload;
    return X_True;
}

X_Boolean cp_dri_init(const s_cp_manager * p_manager,cp_addition_op addition_init)
{
    if(p_manager == (s_cp_manager *)0)  {return X_False;}
    *p_manager ->p_is_init_ok = 0;

    if(p_manager ->event_deepth_backup != CP_EVENT_HSM_DEEPTH
       || p_manager ->send_deepth_backup != CP_SEND_HSM_DEEPTH
       || p_manager ->recv_deepth_backup != CP_RECV_HSM_DEEPTH)
    {
        // report the error to user 
        return X_False;
    }
    //////////if(p_manager ->init_deepth > MAX_HIERARCHICAL_DEEPTH) {return X_False;}
    //////////if(p_manager ->recv_deepth > MAX_HIERARCHICAL_DEEPTH) {return X_False;}
    //////////if(p_manager ->send_deepth > MAX_HIERARCHICAL_DEEPTH) {return X_False;}
    ///////////if(p_manager ->max_send_length == 0)
    if(reset_param(p_manager) != X_True) {return X_False;}

    p_manager ->p_state->cur_state = CP_handle;
    ////////////// p_manager ->p_state->state_backup = CP_nobackup;

    if(addition_init != (cp_addition_op)0) {addition_init(p_manager);}
    *p_manager ->p_is_init_ok = 1;
    return X_True;
}

X_Boolean cp_dri_reset(const s_cp_manager * p_manager,cp_addition_op addition_init)
{
    if(p_manager == (s_cp_manager *)0)  {return X_False;}
    if(*p_manager ->p_is_init_ok == 0)  {return X_False;}
    if(addition_init != (cp_addition_op)0) {addition_init(p_manager);}
    p_manager ->p_state->cur_state = CP_idle;
    ///////////////p_manager ->p_state->state_backup = CP_nobackup;

    if(p_manager ->event_num > CP_MAX_EVENT_NUM)  {return X_False;}

    return X_True;
}

X_Void cp_dri_handle(const s_cp_manager * p_manager,uint32_t ms) // call it in infinite loop situation 
{
    uint8_t cur_byte = 0,get_data_flag = 0;
    if(p_manager == (s_cp_manager *)0)  {return;}
    if(*p_manager ->p_is_init_ok == 0)  {return;}
    if(*p_manager ->p_is_freeze != 0)  {return;}

    switch(p_manager ->p_state->cur_state)
    {
        case CP_idle:
            //////insert_LogDebugOnce(1,p_once,CP_idle,(" ~~~~~~~~~~~~~~~ CP_idle\r\n"));
            reset_param(p_manager);
            p_manager ->p_state->cur_state = CP_handle;        
         break;
        case CP_event:
            //////////insert_LogDebugOnce(1,p_once,CP_init,(" ~~~~~~~~~~~~~~~ CP_init\r\n"));
            p_manager ->p_state->cur_state = CP_handle;
            if(p_manager ->event_table == (s_active_event_param *)0 || p_manager ->event_num == 0) { break; }
            if(*p_manager ->p_cur_event_index >= p_manager ->event_num)  { break; }
            if(p_manager ->event_table[*p_manager ->p_cur_event_index].event == (hsm_task_func)0) { break; }

            if(p_manager ->get_byte_func != (get_byte)0)
            {
                if(*p_manager ->p_query_freq == 0)
                {
                    *p_manager ->p_query_freq = p_manager ->query_freq_reload;
                    get_data_flag = (p_manager ->get_byte_func(&cur_byte) == X_True) ? 1 : 0;
                }
                else
                {
                    (*p_manager ->p_query_freq) --;
                }
                
            }

            p_manager ->p_event_param ->is_new_data_come = get_data_flag;
            p_manager ->p_event_param ->new_data         = cur_byte;
            if(p_manager ->event_table[*p_manager ->p_cur_event_index].event(ms,
                                    &p_manager ->p_event_param->base[CP_EVENT_HSM_DEEPTH]) == X_True)
            {
                p_manager ->p_state->cur_state = CP_event;  // state in current state 
            }
         break;
        case CP_handle:
            ////////insert_LogDebugOnce(1,p_once,CP_handle,(" ~~~~~~~~~~~~~~~ CP_handle\r\n"));
            if(p_manager ->get_byte_func != (get_byte)0)
            {
                if(*p_manager ->p_query_freq == 0)
                {
                    *p_manager ->p_query_freq = p_manager ->query_freq_reload;
                    get_data_flag = (p_manager ->get_byte_func(&cur_byte) == X_True) ? 1 : 0;
                }
                else
                {
                    (*p_manager ->p_query_freq) --;
                }
            }
            send_handle(p_manager,ms,get_data_flag,cur_byte);
            recv_handle(p_manager,ms,get_data_flag,cur_byte);
            if(p_manager ->pass_event != (passive_evt_interface)0) {p_manager ->pass_event(ms,get_data_flag,cur_byte);}
         break;
        default:
            p_manager ->p_state->cur_state = CP_idle;
         break;
    } 
}

X_Void cp_dri_freeze(const s_cp_manager * p_manager)
{
    if(p_manager == (s_cp_manager *)0)  {return;}
    if(*p_manager ->p_is_init_ok == 0)  {return;}
    *p_manager ->p_is_freeze = X_True;
    
}
X_Void cp_dri_unfreeze(const s_cp_manager * p_manager)
{
    if(p_manager == (s_cp_manager *)0)  {return;}
    if(*p_manager ->p_is_init_ok == 0)  {return;}
    *p_manager ->p_is_freeze = X_False;
}

X_Void cp_SendAgain(const s_cp_manager *p_manager)
{
    if(p_manager == (s_cp_manager *)0)  {return;}
    if(*p_manager ->p_is_init_ok == 0)  {return;}
    *p_manager ->p_is_send_again = 1;
}

X_Boolean cp_SendRequestWithResponse(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb
																	,uint32_t rec_time_in_ms,cp_send_rec_cb p_recv_cb)
{
    uint16_t buf_num;
    if(p_manager == (s_cp_manager *)0)  {return X_False;}
    if(*p_manager ->p_is_init_ok == 0)  {return X_False;}

	if(p_data == (uint8_t *)0 || length > p_manager ->max_send_length ) {return X_False;}

	buf_num = LoopQueueFirstIn(p_manager ->p_send_list,X_False);

	if(buf_num < p_manager ->send_cache_num)
	{
        CopyBuffer(p_data,&p_manager ->p_send_buf[buf_num * p_manager ->max_send_length],length);
        p_manager ->p_send_addition[buf_num].send_length            = length;
        p_manager ->p_send_addition[buf_num].rec_time_limit_in_ms   = rec_time_in_ms;
        p_manager ->p_send_addition[buf_num].send_cb                = p_send_cb;
        p_manager ->p_send_addition[buf_num].recv_cb                = p_recv_cb;
		return X_True;
	}
	//INSERT(LogDebug)(LORA_MODULE_DEBUG,(" ------ Lora get a send request but no buf to store it !!!\r\n"));
    // to do later : report the no buf event to user 
	return X_False;
}
X_Boolean cp_SendRequest(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length)
{
    return cp_SendRequestWithResponse(p_manager,p_data,length,(cp_send_rec_cb)0,0,(cp_send_rec_cb)0);
}
X_Boolean cp_SendRequestWithCallBack(const s_cp_manager *p_manager,const uint8_t *p_data,uint16_t length,cp_send_rec_cb p_send_cb)
{
   return cp_SendRequestWithResponse(p_manager,p_data,length,p_send_cb,0,(cp_send_rec_cb)0);
}

X_Boolean cp_DoesNewDataCome(const s_cp_manager *p_manager,uint8_t **pp_data,uint16_t *p_length,uint32_t *p_ms)
{
    uint16_t buf_num;
    if(p_manager == (s_cp_manager *)0)  {return X_False;}
    if(*p_manager ->p_is_init_ok == 0)  {return X_False;}
    
	if(pp_data == (uint8_t **)0 || p_length == (uint16_t *)0) {return X_False;}
	if(DoesLoopQueueEmpty(p_manager ->p_recv_list) == X_True){return X_False;}
	buf_num = LoopQueueFirstOut(p_manager ->p_recv_list);
	if(buf_num < p_manager ->recv_cache_num)
	{
        RealseLoopQueueBuf(p_manager ->p_recv_list,buf_num); 
		
		CopyBuffer(&p_manager ->p_recv_buf[buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)],p_length,CP_LENGTH_SIZE_IN_BYTE);
//////////////		INSERT(LogDebug)(BC95_CLOUD_SCAN_DEBUG,(" ~~~~~~~~NewCommandCome~~~~ %d ; %2x %2x %2x\r\n",length,*pp_data[0],*pp_data[1],*pp_data[2] ));
		
		if(*p_length == 0 || *p_length  > p_manager ->p_recv_param ->max_length)  {return X_False;}

        if(p_ms != (uint32_t *)0)
        {
            CopyBuffer(&p_manager ->p_recv_buf[(buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)) + CP_LENGTH_SIZE_IN_BYTE],p_ms,CP_TIMESTAMP_SIZE_IN_BYTE);
        }

        *pp_data = &p_manager ->p_recv_buf[(buf_num*(p_manager ->p_recv_param ->max_length + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE)) + CP_LENGTH_SIZE_IN_BYTE + CP_TIMESTAMP_SIZE_IN_BYTE];
		return X_True;
	}
	return X_False;
}

X_Boolean cp_active_event(const s_cp_manager *p_manager,uint16_t event_id)
{
    uint16_t i;
    if(p_manager == (s_cp_manager *)0)  {return X_False;}
    if(*p_manager ->p_is_init_ok == 0)  {return X_False;}
    if(p_manager ->event_table == (s_active_event_param *)0)  {return X_False;}
    for(i=0;i< p_manager ->event_num ;i++)
    {
        if(event_id == p_manager ->event_table[i].event_id)
        {
            if(hsm_param_init(&p_manager ->p_event_param ->base[CP_EVENT_HSM_DEEPTH],CP_EVENT_HSM_DEEPTH) != X_True) {return X_False;}
            p_manager ->p_event_param->is_new_data_come = 0;
            p_manager ->p_event_param->new_data         = 0;

            *p_manager ->p_cur_event_index = i;
            p_manager ->p_state->cur_state = CP_event;
            return X_True;
        }
    }
    return X_False;
}
/*
X_Boolean cp_DoesBusy(const s_cp_manager *p_manager) // 职责链或者装饰者？  // 由用户自己去判断模块是否busy ，does initing , dose sending , does recving , does module free ....
{
    return X_False;
}



#define START_ADDR       0

typedef void (*jump_to_app)(void);

jump_to_app jump_to_app_func = (jump_to_app )START_ADDR;

jump_to_app_func();  // (*(void (*) ( ))0)( );

*/

