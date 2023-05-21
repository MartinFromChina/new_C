#include "model.h"
#include "../../../../../module/common/AppCommon.h"
#include "mApp_MainTask.h"
#include "mTask.h"

#define  IMPLEMENT_OF_model     1

static X_Boolean Constructor(model * p_this);
static X_Void Runner(uint32_t ms);
static X_Void Updata(e_basic_key_evt evt,uint8_t key_id);
static X_Boolean ViewObserverReg(view_updata updata_func);
static X_Void Init(X_Void);
static X_Void TimeUp(X_Void);
static X_Void TimeDown(X_Void);
static X_Void TimeNotify(X_Void);
////////static uint32_t GetSpeed(X_Void);

model model1 ={
    // public
	{
        Constructor,
        Updata,
        Runner,
        ViewObserverReg,
        Init,
        TimeUp,
        TimeDown,
        TimeNotify,
////////        GetSpeed,
	},
//////	// protect  :init protect member in construct func 
//////    {0},
    // private  :init private member in construct func
    {0},
};

static X_Boolean Constructor(model * p_this)
{
    if(p_this == (model *)0) {return X_False;}
    PRIVATE_INTERNAL(model,p_this,p_pri);
    
    if(P_THIS(p_this).runner == X_Null){return X_False;}
    
    p_pri ->observer_num = 0;
    for(uint8_t i = 0;i<MAX_VIEW_OBSERVER_NUM;i++)
    {
        p_pri ->view_observer[i] = X_Null;
    }
    
    p_pri ->is_evt_come = X_False;
    p_pri ->cur_view_num = 1;
    p_pri ->cur_ve       = ve_idle;
    p_pri ->delay_ms     = 21600000;
    p_pri ->is_config_begin = X_False;
    p_pri ->speed           = 30000;
    p_pri ->notify_param_sw = 0;
    return X_True;
}

static X_Void Runner(uint32_t ms)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    
    if(p_pri ->is_evt_come == X_False)  {return;}
    p_pri ->is_evt_come = X_False;
    for(uint8_t i = 0;i<MAX_VIEW_OBSERVER_NUM;i++)
    {
        if(p_pri ->view_observer[i] != X_Null)
        {
            p_pri ->view_observer[i](p_pri ->cur_ve,p_pri ->cur_view_num,
            (p_pri ->notify_param_sw == 0) ? p_pri ->delay_ms:p_pri ->speed);
        }
    }
    
    p_pri ->notify_param_sw = 0;
}
/*
key1 long push : time ++
key2 long push : time --
key1 double click : set alarm clock 

key1 click : speed++
key2 click : speed--
*/
static X_Void Updata(e_basic_key_evt evt,uint8_t key_id)
{
    SysLogDebug(1,(" modle1 key %d evt %d \r\n",key_id,evt));
    PRIVATE_INTERNAL(model,&model1,p_pri);
    
    if(key_id == 1 && evt == ke_long_push)
    {
        if(p_pri ->is_config_begin == X_False)
        {
           p_pri ->is_config_begin = X_True;
           
           p_pri ->is_evt_come = X_True;
           p_pri ->cur_ve      = ve_config_clock;
            
           common_task_freeze(TID_MainTask);
            
           common_task_restart(TID_TimeUp);
           common_task_freeze(TID_TimeDown);
        }
        
        
    }
    else if(key_id == 2 && evt == ke_long_push)
    {
        if(p_pri ->is_config_begin == X_False)
        {
           p_pri ->is_config_begin = X_True;
            
           p_pri ->is_evt_come = X_True;
           p_pri ->cur_ve      = ve_config_clock;
            
            common_task_freeze(TID_MainTask);
            common_task_restart(TID_TimeDown);//
            common_task_freeze(TID_TimeUp);
        }
        
       
    }
    else if((key_id == 1 || key_id == 2) && evt == ke_long_push_release)
    {
        p_pri ->is_config_begin = X_False;
        common_task_freeze(TID_TimeUp);
        common_task_freeze(TID_TimeDown);
    }
    else if(key_id == 1 && evt == ke_double_click)
    {
        p_pri ->is_evt_come = X_True;
        p_pri ->cur_ve      = ve_set_alarm_clock;
        p_pri ->is_config_begin = X_False;
        
        SetCurDelayMs(p_pri ->delay_ms);
        common_task_restart(TID_MainTask);
        common_task_freeze(TID_TimeUp);
        common_task_freeze(TID_TimeDown);
    }
    else if(key_id == 1 && evt == ke_click)
    {
        p_pri ->is_evt_come = X_True;
        p_pri ->cur_ve      = ve_config_speed;
        p_pri ->notify_param_sw = 1;
        p_pri ->speed += 1000;
    }
    else if(key_id == 2 && evt == ke_click)
    {
        p_pri ->is_evt_come = X_True;
        p_pri ->cur_ve      = ve_config_speed;
        p_pri ->notify_param_sw = 1;
        if(p_pri ->speed >= 1000) {p_pri ->speed -= 1000;}
    }
}

static X_Boolean ViewObserverReg(view_updata updata_func)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    if(p_pri ->observer_num > MAX_VIEW_OBSERVER_NUM || updata_func == X_Null) {return X_False;}
    p_pri ->view_observer[p_pri ->observer_num ++] = updata_func;
    return X_True;
}

static X_Void Init(X_Void)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    for(uint8_t i = 0;i<MAX_VIEW_OBSERVER_NUM;i++)
    {
        if(p_pri ->view_observer[i] != X_Null)
        {
            p_pri ->view_observer[i](ve_time_updata,p_pri ->cur_view_num,p_pri ->delay_ms);
            p_pri ->view_observer[i](ve_config_speed,p_pri ->cur_view_num,p_pri ->speed);
        }
    }
    p_pri ->is_evt_come = X_True;
    p_pri ->cur_ve      = ve_set_alarm_clock;
    SetCurDelayMs(p_pri ->delay_ms);
    common_task_restart(TID_MainTask);
}

static X_Void TimeUp(X_Void)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    p_pri ->delay_ms += p_pri ->speed;
}
static X_Void TimeDown(X_Void)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    if(p_pri ->delay_ms >= p_pri ->speed){p_pri ->delay_ms  -= p_pri ->speed;}
}
static X_Void TimeNotify(X_Void)
{
    PRIVATE_INTERNAL(model,&model1,p_pri);
    for(uint8_t i = 0;i<MAX_VIEW_OBSERVER_NUM;i++)
    {
        if(p_pri ->view_observer[i] != X_Null)
        {
            p_pri ->view_observer[i](ve_time_updata,p_pri ->cur_view_num,p_pri ->delay_ms);
        }
    }
}

////////static uint32_t GetSpeed(X_Void)
////////{

////////}
