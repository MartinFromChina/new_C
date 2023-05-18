#include "controller.h"
#include "../../../../../module/common/AppCommon.h"

#define  IMPLEMENT_OF_controller     1

static X_Boolean Constructor(controller * p_this);
static X_Void KeyEvent(uint8_t key_id,e_basic_key_evt evt,uint32_t ms);
static X_Void KeySubject(X_Void);

controller controller_basic = {
    // public
	{
        Constructor,
        KeyEvent,
        KeySubject,
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
        p_pri ->key_evt_table[i].value = ke_idle;
        p_pri ->key_evt_table[i].updata = X_Null;
    }
    
    
    return X_True;
}

static X_Void KeyEvent(uint8_t key_id,e_basic_key_evt evt,uint32_t ms)
{
    ms = ms;
    if(key_id == 0 || key_id > MAX_BUTTON_NUM) {return;}
    PRIVATE_INTERNAL(controller,&controller_basic,p_pri);
    
    p_pri ->key_evt_table[key_id - 1].value = evt;
}

static X_Void KeySubject(X_Void)
{
    PRIVATE_INTERNAL(controller,&controller_basic,p_pri);
    for(uint8_t i=0;i<MAX_BUTTON_NUM;i++)
    {
        if(p_pri ->key_evt_table[i].value != ke_idle)
        {
            if(p_pri ->key_evt_table[i].updata != X_Null)
            {
                p_pri ->key_evt_table[i].updata(p_pri ->key_evt_table[i].value);
            }
        }
    }
}

#undef  IMPLEMENT_OF_controller
