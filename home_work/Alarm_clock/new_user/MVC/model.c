#include "model.h"
#include "../../../../../module/common/AppCommon.h"

static X_Boolean Constructor(model * p_this);
static X_Void Runner(uint32_t ms);
static X_Void Updata(e_basic_key_evt evt,uint8_t key_id);

model modle1 ={
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

static X_Boolean Constructor(model * p_this)
{
    if(p_this == (model *)0) {return X_False;}
//////////    PRIVATE_INTERNAL(model,p_this,p_pri);
    
    if(P_THIS(p_this).runner == X_Null){return X_False;}
    
    return X_True;
}

static X_Void Runner(uint32_t ms)
{

}

static X_Void Updata(e_basic_key_evt evt,uint8_t key_id)
{
    SysLogDebug(1,(" modle1 key %d evt %d \r\n",key_id,evt));
}
