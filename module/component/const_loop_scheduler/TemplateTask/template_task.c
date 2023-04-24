#include "template_task.h"
/********************************************************************************************/
e_x_task_error_code t_task_init(const s_T_Tasks_Manager *p_manager)
{
    uint16_t i;
    if(p_manager == (s_T_Tasks_Manager *)0) 
    {
        return xte_pointer_null;
    }
    if(p_manager ->instance_num > MAX_TEMPLATE_INSTANCE_NUM  
        || p_manager ->instance_with_name_num > MAX_TEMPLATE_INSTANCE_NUM 
        || p_manager ->instance_num == 0) 
    {
        return xte_beyond_scope;
    }
    if(p_manager ->p_template_task_func == (t_task_func)0)  //X_TASK_LOG(1,(" fatal init error !!! : task list null \r\n"));
    {
        return xte_pointer_null;
    }

  //  fix it later : 此处可以将每个任务的task id 排序，然后依次比较，发现有相同的相邻的ID就返回 FALSE ，稍后再做
  //  或者干脆就用枚举类型做task ID,这样就不会重复了
    for(i=0;i<p_manager -> instance_num;i++)
    {
        p_manager ->p_freeze_flag_buf[i] = 0;
        p_manager ->p_init_flag_buf[i]   = 0;
    } 
    
    *p_manager ->p_is_init_ok = 1;//X_TASK_LOG(1,(" ~~~~~~ task init success ~~~~~~~ ; total task instance number %d \r\n",p_manager -> instance_num));
    return xte_ok;
}

e_x_task_error_code  t_task_scheduler(const s_T_Tasks_Manager *p_manager,uint32_t ms)
{
    uint16_t i;

    if(p_manager == (s_T_Tasks_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal scheduler error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; //X_TASK_LOG(1,(" fatal scheduler error !!! : init failed \r\n"));
    }
     
    for(i=0;i<p_manager ->instance_num;i++)
    {
        if(p_manager ->p_freeze_flag_buf[i] == 0)
        {
            p_manager ->p_template_task_func(
                ms,
                &(p_manager ->p_init_flag_buf[i]),
                p_manager ->p_param_buf[i],
                i,
                (i >= p_manager ->instance_with_name_num ) ? (uint8_t *)"noname " : &p_manager ->p_instance_name_buf[(i + 1)*MAX_TEMPLATE_INSTANCE_NAME_LENGTH]
            );
        }
    }
	return xte_ok;
}

e_x_task_error_code freeze_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id)
{
    if(p_manager == (s_T_Tasks_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal freeze error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; //X_TASK_LOG(1,(" fatal freeze error !!! : init failed \r\n"));
    }

    if(instance_id >= p_manager ->instance_num)
    {
        return xte_beyond_scope;
    }

    p_manager ->p_freeze_flag_buf[instance_id] = 1;

    return xte_ok;
}
e_x_task_error_code unfreeze_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id)
{
    if(p_manager == (s_T_Tasks_Manager *)0) 
    {
        return xte_pointer_null; //X_TASK_LOG(1,(" fatal unfreeze error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed; // X_TASK_LOG(1,(" fatal unfreeze error !!! : init failed \r\n"));
    }

    if(instance_id >= p_manager ->instance_num)
    {
        return xte_beyond_scope;
    }

    p_manager ->p_freeze_flag_buf[instance_id] = 0;

    return xte_ok;
}
e_x_task_error_code restart_t_task(const s_T_Tasks_Manager *p_manager,uint16_t instance_id)
{
    if(p_manager == (s_T_Tasks_Manager *)0) 
    {
        return xte_pointer_null;   ////////X_TASK_LOG(1,(" fatal restart error !!! : task manager pointer null \r\n"));
    }
    if(*p_manager ->p_is_init_ok == 0) 
    {
        return xte_init_failed;  /////////X_TASK_LOG(1,(" fatal restart error !!! : init failed \r\n"));
    }

    if(instance_id >= p_manager ->instance_num)
    {
        return xte_beyond_scope;
    }

    p_manager ->p_freeze_flag_buf[instance_id] = 0;
    p_manager ->p_init_flag_buf[instance_id] = 0;

    return  xte_ok;
}
