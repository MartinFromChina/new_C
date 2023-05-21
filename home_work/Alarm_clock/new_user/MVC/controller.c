#include "controller.h"
#include "../../../../../module/common/AppCommon.h"

#define  IMPLEMENT_OF_controller     1

static X_Boolean Constructor(controller * p_this);
static X_Void KeyEvent(uint8_t key_id,e_basic_key_evt evt,uint32_t ms);
static X_Void KeySubject(X_Void);
static X_Boolean KeyObserverReg(key_updata updata_func);

controller controller_basic = {
    // public
	{
        Constructor,
        KeyEvent,
        KeySubject,
        KeyObserverReg,
	},
	// protect  :init protect member in construct func 
    //{0},
    // private  :init private member in construct func
    {0},

};

static X_Boolean Constructor(controller * p_this)
{
    if(p_this == (controller *)0) {return X_False;}
    PRIVATE_INTERNAL(controller,p_this,p_pri);
    
    if(P_THIS(p_this).key_evt == X_Null){return X_False;}
    
    for(uint8_t i = 0;i<MAX_BUTTON_NUM;i++)
    {
        p_pri ->key_evt_table[i] = ke_idle;
    }
    
    for(uint8_t i = 0;i<MAX_KEY_OBSERVER_NUM;i++)
    {
        p_pri ->key_observer[i] = X_Null;
    }
    p_pri ->observer_num = 0;
    return X_True;
}

static X_Void KeyEvent(uint8_t key_id,e_basic_key_evt evt,uint32_t ms)
{
    ms = ms;
    if(key_id == 0 || key_id > MAX_BUTTON_NUM) {return;}
    PRIVATE_INTERNAL(controller,&controller_basic,p_pri);
    
    p_pri ->key_evt_table[key_id - 1] = evt;
}

static X_Void KeySubject(X_Void)
{
    PRIVATE_INTERNAL(controller,&controller_basic,p_pri);
    for(uint8_t i=0;i<MAX_BUTTON_NUM;i++)
    {
        if(p_pri ->key_evt_table[i] != ke_idle)
        {
             for(uint8_t j = 0;j<MAX_KEY_OBSERVER_NUM;j++)
            {
                if(p_pri ->key_observer[j] != X_Null) 
                {
                    p_pri ->key_observer[j](p_pri ->key_evt_table[i],i+1);
                }
                
            }
            
            p_pri ->key_evt_table[i] = ke_idle;
        }
    }
}

static X_Boolean KeyObserverReg(key_updata updata_func)
{
    PRIVATE_INTERNAL(controller,&controller_basic,p_pri);
    if(p_pri ->observer_num > MAX_KEY_OBSERVER_NUM || updata_func == X_Null) {return X_False;}
    p_pri ->key_observer[p_pri ->observer_num ++] = updata_func;
    return X_True;
}

#undef  IMPLEMENT_OF_controller
