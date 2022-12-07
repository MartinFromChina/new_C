#include "thread_in_loop.h"

X_Void AppThreadToDo(const sThreadInLoop *p_manager,one_step todo,uint32_t cur_line_num)
{
    if(p_manager != (sThreadInLoop*)0)	           
    {
        if ((*p_manager ->p_cur_line_num) <  cur_line_num)  
        {                                                             
            *p_manager ->p_cur_line_num = cur_line_num;          
            if( todo != (one_step)0) {todo();}               
        }                                                       
    }                                   	                
}

X_Void AppThreadEndHandle(const sThreadInLoop *p_manager)
{
    if(p_manager != (sThreadInLoop*)0)	
    {
        *p_manager ->p_cur_line_num = 0;  
    } 
}

X_Boolean DoesAppThreadWait(const sThreadInLoop *p_manager,uint32_t wait_time_ms,uint32_t cur_line_num)
{
    if(wait_time_ms == 0) { return X_False;}
    if(p_manager != (sThreadInLoop*)0)	
    {
        if ((*p_manager ->p_cur_line_num) < cur_line_num)  
        {
            *p_manager ->p_cur_line_num = cur_line_num;  
            if(p_manager ->p_set_time != (set_time)0) 
            {
                p_manager ->p_set_time(wait_time_ms);
            }
            return X_True;
        }
        else if( *p_manager ->p_cur_line_num == cur_line_num)
        {
            if(p_manager ->p_get_time != (get_time)0)
            {
                if(p_manager ->p_get_time() == 0) {return X_False;}
                else {return X_True;}
            }
            else
            {
                return X_False;
            }

        }
        else
        {
            return X_False;
        }
    }
    return X_False;
}


X_Boolean DoesAppThreadWaitUntil(const sThreadInLoop *p_manager,uint32_t cur_line_num,one_step init_func,does_in_conditon in_conditon_func)
{
    if(in_conditon_func == (does_in_conditon)0) { return X_False;}
    if(p_manager != (sThreadInLoop*)0)	
    {
        if ((*p_manager ->p_cur_line_num) < cur_line_num)  
        {
            *p_manager ->p_cur_line_num = cur_line_num;  
            if(init_func != (one_step)0) 
            {
                init_func();
            }
            return X_True; // still wait 
        }
        else if((*p_manager ->p_cur_line_num) == cur_line_num)
        {
            if(in_conditon_func() == X_True) {return X_False;} // not wait anymore 
            else {return X_True;} // still wait 
        }
        else
        {
            return X_False;
        }
        
    }
    return X_False;
}

static X_Void LoopTodoWithDelay(const sThreadInLoop *p_manager,one_step todo,uint32_t delay_ms) 
{
    if(delay_ms == 0)
    {
        if(todo != (one_step)0) {todo();}
        return ;
    }

    if((*p_manager ->p_state_sw) == 0)
    {
        *p_manager ->p_state_sw = 1;
        if(p_manager ->p_set_time != (set_time)0) {p_manager ->p_set_time(delay_ms);}
        if(todo != (one_step)0) {todo();}
    }
    else
    {
        if(p_manager ->p_get_time == (get_time)0) 
        {
            *p_manager ->p_state_sw = 0;
            if(todo != (one_step)0) {todo();}
        }
        else
        {
            if(p_manager ->p_get_time() == 0) {*p_manager ->p_state_sw = 0;}
        }
    }
}
X_Boolean DoesAppThreadWhileLoop(const sThreadInLoop *p_manager,uint32_t cur_line_num
                                ,one_step init_func,does_in_conditon in_conditon_func,one_step todo,uint32_t step_delay_ms)
{
    if(p_manager != (sThreadInLoop*)0)	
    {
        if ((*p_manager ->p_cur_line_num) < cur_line_num)  
        {
            *p_manager ->p_cur_line_num = cur_line_num;  
            if(init_func != (one_step)0) {init_func();}
            *p_manager ->p_state_sw     = 0;
            return X_True; // still wait 
        }
        else if((*p_manager ->p_cur_line_num) == cur_line_num)
        {
            if(in_conditon_func != (does_in_conditon)0)
            {
                if(in_conditon_func() == X_True) {return X_False;} // not wait anymore 
                else 
                {
                    LoopTodoWithDelay(p_manager,todo,step_delay_ms);
                    return X_True; // still wait 
                } 
            }
            else // infinite loop
            {
                LoopTodoWithDelay(p_manager,todo,step_delay_ms);
                return X_True; // still wait 
            }
            
        }
        else
        {
            return X_False;
        }
    }
    return X_False;

}

static X_Boolean ForTodoWithDelay(const sThreadInLoop *p_manager,one_step todo,uint32_t delay_ms) //  return true : still wait , return false : not wait any more
{
    if(delay_ms == 0)
    {
        if(todo != (one_step)0) {todo();}
        if((*p_manager ->p_for_loop_cnt) > 0)
        {
            (*p_manager ->p_for_loop_cnt) --;
            return ((*p_manager ->p_for_loop_cnt) != 0); 
        }
        else
        {
            return X_False;
        }
    }

    if((*p_manager ->p_state_sw) == 0)
    {
        *p_manager ->p_state_sw = 1;
        if(p_manager ->p_set_time != (set_time)0) {p_manager ->p_set_time(delay_ms);}
        if(todo != (one_step)0) {todo();}
        if((*p_manager ->p_for_loop_cnt) > 0)
        {
            (*p_manager ->p_for_loop_cnt) --;
            return ((*p_manager ->p_for_loop_cnt) != 0); 
        }
        else
        {
            return X_False;
        }
    }
    else
    {
        if(p_manager ->p_get_time == (get_time)0) 
        {
            *p_manager ->p_state_sw = 0;
            if(todo != (one_step)0) {todo();}
            if((*p_manager ->p_for_loop_cnt) > 0)
            {
                (*p_manager ->p_for_loop_cnt) --;
                return ((*p_manager ->p_for_loop_cnt) != 0); 
            }
            else
            {
                return X_False;
            }
        }
        else
        {
            if(p_manager ->p_get_time() == 0) {*p_manager ->p_state_sw = 0;}
        }
        return X_True; 
    }
}
X_Boolean DoesAppThreadForLoop(const sThreadInLoop *p_manager,uint32_t cur_line_num
                                ,one_step init_func,does_in_conditon in_conditon_func,one_step todo,uint32_t step_delay_ms,uint32_t loop_times)
{
    if(p_manager != (sThreadInLoop*)0)	
    {
        if ((*p_manager ->p_cur_line_num) < cur_line_num)  
        {
            *p_manager ->p_cur_line_num = cur_line_num;  
            if(init_func != (one_step)0) {init_func();}
            *p_manager ->p_state_sw     = 0;
            *p_manager ->p_for_loop_cnt = loop_times;
            return X_True; // still wait 
        }
        else if((*p_manager ->p_cur_line_num) == cur_line_num)
        {
            if(in_conditon_func != (does_in_conditon)0)
            {
                if(in_conditon_func() == X_True) {return X_False;} // not wait anymore 
                else 
                {
                   // if(ForTodoWithDelay(p_manager,todo,step_delay_ms) == X_True) {return X_True;} // still wait 
                   // else {return X_False;} 
                   return ForTodoWithDelay(p_manager,todo,step_delay_ms);
                } 
            }
            else // infinite loop
            {
                return ForTodoWithDelay(p_manager,todo,step_delay_ms);
            }
            
        }
        else
        {
            return X_False;
        }
    }
    return X_False;

}
