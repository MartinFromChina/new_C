#include "loop_scheduler.h"
/********************************************************************************************/
typedef struct 
{
    X_Boolean isFrozen;
    uint16_t  id;
    x_task_func task_process;
    X_Boolean *isInitDone;
}sTaskList;

static x_get_time  sys_get_ms = (x_get_time)0;
static sTaskList task_list[TOTAL_X_TASK_NUMBER];
static uint32_t total_task_num = 0;

X_Boolean task_scheduler_init(x_get_time p_get_time_func)
{
    uint16_t i;
    total_task_num = 0;

    for(i=0;i<TOTAL_X_TASK_NUMBER;i++)
    {
        task_list[i].isFrozen     = X_False;
        task_list[i].isInitDone   = (X_Boolean*)0;
        task_list[i].id           = 0xffff;
        task_list[i].task_process = (x_task_func)0;
    }

    if(p_get_time_func == (x_get_time)0) {return X_False;}
    sys_get_ms = p_get_time_func;
    return X_True;
}

X_Boolean add_task_to_schedule(uint16_t task_id,x_task_func p_task_func,X_Boolean *p_isInit)
{
    if(total_task_num >= TOTAL_X_TASK_NUMBER) {return X_False;}
		if(task_id == 0xffff) {return X_False;}
    if(p_task_func == (x_task_func)0) {return X_False;}

    task_list[total_task_num].isFrozen     = X_False;
    task_list[total_task_num].isInitDone   = p_isInit;
    task_list[total_task_num].id           = task_id;
    task_list[total_task_num].task_process = p_task_func;

    total_task_num ++;

    return X_True;
}

X_Void  loop_task_scheduler(X_Void)
{
    uint32_t cur_ms;
    uint16_t i;
    if(sys_get_ms == (x_get_time)0) {return;}
    if(total_task_num == 0 || total_task_num > TOTAL_X_TASK_NUMBER) {return;}
    cur_ms = sys_get_ms();

    for(i=0;i<total_task_num;i++)
    {
        if(task_list[i].isFrozen != X_True && task_list[i].task_process != (x_task_func)0)
        {
            task_list[i].task_process(cur_ms,task_list[i].id);
        }
    }
}

X_Boolean freeze_task_in_schedule(uint16_t task_id)
{
    uint16_t i;
    X_Boolean isOk = X_False;
    if(total_task_num == 0 || total_task_num > TOTAL_X_TASK_NUMBER) {return X_False;}
    for(i=0;i<total_task_num;i++)
    {
        if(task_list[i].id == task_id)
        {
            task_list[i].isFrozen = X_True;
            isOk = X_True;
            break;
        }
    }
    return isOk;
}
X_Boolean unfreeze_task_in_schedule(uint16_t task_id)
{
    uint16_t i;
    X_Boolean isOk = X_False;
    if(total_task_num == 0 || total_task_num > TOTAL_X_TASK_NUMBER) {return X_False;}
    for(i=0;i<total_task_num;i++)
    {
        if(task_list[i].id == task_id)
        {
            task_list[i].isFrozen = X_False;
            isOk = X_True;
            break;
        }
    }
    return isOk;
}
X_Boolean restart_task_in_schedule(uint16_t task_id)
{
    uint16_t i;
    X_Boolean isOk = X_False;
    if(total_task_num == 0 || total_task_num > TOTAL_X_TASK_NUMBER) {return X_False;}
    if(task_list[task_id].isInitDone == (X_Boolean*)0) {return X_False;}
    for(i=0;i<total_task_num;i++)
    {
        if(task_list[i].id == task_id)
        {
            task_list[i].isFrozen = X_False;
            *task_list[i].isInitDone = X_False;
            isOk = X_True;
            break;
        }
    }
    return isOk;
}

X_Void X_Task_ByteBufClear(uint8_t *p_buf,uint16_t length)
{
	uint16_t i;
    if(length == 0 || length == 0xffff ) {return;}

	for(i = 0;i<length;i++)
	{
		p_buf[i] = 0;
	}
}
X_Void X_Task_32bitsBufClear(uint32_t *p_buf,uint16_t length)
{
	uint16_t i;
    if(length == 0 || length == 0xffff ) {return;}

	for(i = 0;i<length;i++)
	{
		p_buf[i] = 0;
	}
}
/********************************************************************************************/
X_Void X_Task_if_line_push(uint16_t loop_deepth,uint16_t if_deepth,uint8_t *p_line_flag_buf)
{
    // no need length or pointer check ,because the only caller is X_Task Marco 
    if(loop_deepth == 0) {return;}
    p_line_flag_buf[if_deepth] = 1;
}

X_Void X_Task_if_line_pop_All(uint16_t loop_deepth,uint16_t max_if_deepth,uint8_t *p_line_flag_buf,uint32_t *P_line_buf)
{
    uint16_t i;
    // no need length or pointer check ,because the only caller is X_Task Marco 
    if(loop_deepth == 0) {return;}

    for(i=0;i<max_if_deepth;i++)
    {
        if(p_line_flag_buf[i] != 0) 
        {
            p_line_flag_buf[i] = 0;
            P_line_buf[i] = 0;
        }
    }
}

X_Void X_Task_loop_line_pop_all(uint16_t loop_deepth,uint16_t max_loop_deepth,uint32_t *P_end_line_buf,uint32_t *P_line_buf)
{
    uint16_t i;
    // no need length or pointer check ,because the only caller is X_Task Marco 
    ////////////printf("loop pop from %d to %d\r\n",loop_deepth,max_loop_deepth-1);
    for(i=loop_deepth;i<max_loop_deepth;i++)
    {
        P_end_line_buf[i] = 0;
        P_line_buf[i] = 0;
    }
}
/********************************************************************************************/
