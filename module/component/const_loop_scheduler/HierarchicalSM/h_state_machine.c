#include "h_state_machine.h"
/*
typedef struct tage1
{
    uint8_t hehe;
    struct tage1 * const p_level_next;
}s_T_taskBaseParam;

static s_T_taskBaseParam  test_buf[3] = {
    {
        {0},
        .p_level_next = &test_buf[1],
    },
    {
        {0},
        .p_level_next = &test_buf[2],
    },
    {
        {0},
        .p_level_next = (s_T_taskBaseParam*)0,
    },
};
*/
/********************************************************************************************/
X_Void hsm_one_level_param_Init(s_T_taskBaseParam * p_param)
{
    if(p_param == (s_T_taskBaseParam *)0) {return;} // still have the risk of called by the user ,so the null pointer check is necessary 

    p_param ->level_cur.cur_line_num      = 0;                                        
    p_param ->level_cur.ms_backup         = 0;                                           
    p_param ->level_cur.isErrorOccur      = 0;                                                                                                             
    X_Task_ByteBufClear(p_param ->level_cur.if_condition_buf,MAX_HSM_IF_NEST_DEEPTH + 1);           
    X_Task_32bitsBufClear(p_param ->level_cur.if_line_num_buf,MAX_HSM_IF_NEST_DEEPTH + 1);           
    X_Task_ByteBufClear(p_param ->level_cur.loop_condition_buf,MAX_HSM_LOOP_NEST_DEEPTH + 1);           
    X_Task_32bitsBufClear(p_param ->level_cur.loop_line_num_buf,MAX_HSM_LOOP_NEST_DEEPTH + 1);           
    X_Task_32bitsBufClear(p_param ->level_cur.loop_end_line_num_buf,MAX_HSM_LOOP_NEST_DEEPTH + 1);           
    X_Task_ByteBufClear(&p_param ->level_cur.if_line_flag_array[0][0],(MAX_HSM_LOOP_NEST_DEEPTH + 2) *(MAX_HSM_IF_NEST_DEEPTH + 1) );          

}
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
/********************************************************************************************/
e_x_task_error_code hsm_init(const s_HSM_Manager *p_manager)
{
    if(p_manager == (s_HSM_Manager *)0 ) 
    {
        return xte_pointer_null;
    }

    if(p_manager ->p_hsm_func == (hsm_task_func)0 
            || p_manager ->p_is_freeze == (uint8_t *)0 
            || p_manager ->p_is_init_ok == (uint8_t *)0
            || p_manager ->p_hsm_param == (s_T_taskBaseParam *)0)  //X_TASK_LOG(1,(" fatal init error !!! : task list null \r\n"));
    {
        return xte_pointer_null;
    }

    if(p_manager ->h_deepth > MAX_HIERARCHICAL_DEEPTH) {return xte_beyond_scope;}

    if(hsm_param_init(p_manager ->p_hsm_param,p_manager ->h_deepth) != X_True) {return xte_unknow_error;}

    *p_manager ->p_is_freeze = 0;
    *p_manager ->p_is_init_ok = 1;//X_TASK_LOG(1,(" ~~~~~~ task init success ~~~~~~~ ; total task instance number %d \r\n",p_manager -> instance_num));
    return xte_ok;
}

e_x_task_error_code hsm_run(const s_HSM_Manager *p_manager,uint32_t ms)
{
    if(p_manager == (s_HSM_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal scheduler error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; //X_TASK_LOG(1,(" fatal scheduler error !!! : init failed \r\n"));
    }
    if(*p_manager ->p_is_freeze != 0) {return xte_frozen;}
     
    p_manager ->p_hsm_func(ms,p_manager ->p_hsm_param);
	return xte_ok;
}

e_x_task_error_code freeze_hsm(const s_HSM_Manager *p_manager)
{
    if(p_manager == (s_HSM_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal freeze error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; //X_TASK_LOG(1,(" fatal freeze error !!! : init failed \r\n"));
    }

    *p_manager ->p_is_freeze = 1;

    return xte_ok;
}
e_x_task_error_code unfreeze_hsm(const s_HSM_Manager *p_manager)
{
    if(p_manager == (s_HSM_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal unfreeze error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; // X_TASK_LOG(1,(" fatal unfreeze error !!! : init failed \r\n"));
    }

    *p_manager ->p_is_freeze = 0;

    return xte_ok;
}
e_x_task_error_code restart_hsm(const s_HSM_Manager *p_manager)
{
    if(p_manager == (s_HSM_Manager *)0) 
    {
        return xte_pointer_null;   ////////X_TASK_LOG(1,(" fatal restart error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed;  /////////X_TASK_LOG(1,(" fatal restart error !!! : init failed \r\n"));
    }

    *p_manager ->p_is_freeze = 0;

    hsm_param_init(p_manager ->p_hsm_param,p_manager ->h_deepth);

    return  xte_ok;
}

