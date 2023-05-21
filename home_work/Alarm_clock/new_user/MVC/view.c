#include "view.h"
#include "../../../../../module/common/AppCommon.h"

static X_Boolean Constructor(view * p_this);
static X_Void Runner(uint32_t ms);
static X_Void Updata(e_basic_key_evt evt,uint8_t key_id);

view view1 = {
   // public
	{
        Constructor,
        Updata,
        Runner,
	},
//////	// protect  :init protect member in construct func 
//////    {0},
//////    // private  :init private member in construct func
//////    {0},
};



static X_Boolean Constructor(view * p_this)
{
    if(p_this == (view *)0) {return X_False;}
//////////    PRIVATE_INTERNAL(model,p_this,p_pri);
    
////////    if(P_THIS(p_this).runner == X_Null){return X_False;}
    
    return X_True;
}

static X_Void Runner(uint32_t ms)
{
    ms = ms;
}

static X_Void Updata(e_basic_key_evt evt,uint8_t key_id)
{
    SysLogDebug(1,(" view1 key %d evt %d \r\n",key_id,evt));
}
