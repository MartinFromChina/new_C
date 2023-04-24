#include "const_loop_scheduler.h"
/********************************************************************************************/
e_x_task_error_code x_task_init(const s_X_Tasks_Manager *p_manager)
{
    uint16_t i;
    if(p_manager == (s_X_Tasks_Manager *)0) 
    {
        //X_TASK_LOG(1,(" fatal init error !!! : task manager pointer null \r\n"));
        return xte_pointer_null;
    }
    if(p_manager -> max_task_num >= MAX_X_TASK_NUMBER ) 
    {
        //X_TASK_LOG(1,(" fatal init error !!! : task number beyond scope \r\n"));
        return xte_beyond_scope;
    }
    if(p_manager -> p_task == (s_X_TaskList *)0) 
    {
         //X_TASK_LOG(1,(" fatal init error !!! : task list null \r\n"));
        return xte_pointer_null;
    }

  //  fix it later : 此处可以将每个任务的task id 排序，然后依次比较，发现有相同的相邻的ID就返回 FALSE ，稍后再做
  //  或者干脆就用枚举类型做task ID,这样就不会重复了
    for(i=0;i<p_manager -> max_task_num;i++)
    {
        p_manager ->p_freeze_flag_buf[i] = 0;
        p_manager ->p_init_flag_buf[i]   = 0;
    } 

    *p_manager ->p_is_init_ok = 1;
    ///////X_TASK_LOG(1,(" ~~~~~~ task init success ~~~~~~~ ; total task number %d \r\n",p_manager -> max_task_num));
    return xte_ok;
}

e_x_task_error_code  x_task_scheduler(const s_X_Tasks_Manager *p_manager,uint32_t ms)
{
    uint16_t i;

    if(p_manager == (s_X_Tasks_Manager *)0) 
    {
        //X_TASK_LOG(1,(" fatal scheduler error !!! : task manager pointer null \r\n"));
        return xte_pointer_null;
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        //X_TASK_LOG(1,(" fatal scheduler error !!! : init failed \r\n"));
        return xte_init_failed;
    }
     
    for(i=0;i<p_manager -> max_task_num;i++)
    {
        if(p_manager ->p_freeze_flag_buf[i] == 0 &&  p_manager ->p_task[i].task_process !=  (x_task_func)0)
        {
            p_manager ->p_task[i].task_process(ms,p_manager ->p_task[i].id,&(p_manager ->p_init_flag_buf[i]));
        }
    }
	return xte_ok;
}

e_x_task_error_code freeze_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id)
{
    uint16_t i;
    X_Boolean isOk = X_False;

    if(p_manager == (s_X_Tasks_Manager *)0) 
    {
        //X_TASK_LOG(1,(" fatal freeze error !!! : task manager pointer null \r\n"));
        return xte_pointer_null;
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        //X_TASK_LOG(1,(" fatal freeze error !!! : init failed \r\n"));
        return xte_init_failed;
    }

    for(i=0;i<p_manager -> max_task_num;i++)
    {
        if(p_manager ->p_task[i].id == task_id)
        {
            p_manager ->p_freeze_flag_buf[i] = 1;
            isOk = X_True;
            break;
        }
    }
    return ((isOk == X_True) ? xte_ok : xte_unknow_error);
}
e_x_task_error_code unfreeze_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id)
{
    uint16_t i;
	X_Boolean isOk = X_False;
    if(p_manager == (s_X_Tasks_Manager *)0) 
    {
        //X_TASK_LOG(1,(" fatal unfreeze error !!! : task manager pointer null \r\n"));
        return xte_pointer_null;
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
       // X_TASK_LOG(1,(" fatal unfreeze error !!! : init failed \r\n"));
        return xte_init_failed;
    }

    for(i=0;i<p_manager -> max_task_num;i++)
    {
        if(p_manager ->p_task[i].id == task_id)
        {
            p_manager ->p_freeze_flag_buf[i] = 0;
            isOk = X_True;
            break;
        }
    }
   return ((isOk == X_True) ? xte_ok : xte_unknow_error);
}
e_x_task_error_code restart_x_task(const s_X_Tasks_Manager *p_manager,uint16_t task_id)
{
    uint16_t i;
    X_Boolean isOk = X_False;

    if(p_manager == (s_X_Tasks_Manager *)0) 
    {
        ////////X_TASK_LOG(1,(" fatal restart error !!! : task manager pointer null \r\n"));
        return xte_pointer_null;
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        /////////X_TASK_LOG(1,(" fatal restart error !!! : init failed \r\n"));
        return xte_init_failed;
    }

    for(i=0;i<p_manager -> max_task_num;i++)
    {
        if(p_manager ->p_task[i].id == task_id)
        {
            p_manager ->p_freeze_flag_buf[i] = 0;
            p_manager ->p_init_flag_buf[i] = 0;
            isOk = X_True;
            break;
        }
    }
    return ((isOk == X_True) ? xte_ok : xte_unknow_error);
}


