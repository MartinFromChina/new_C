#include "controller.h"

extern controller controller_basic;
X_Boolean ClassInit(X_Void)
{
    FUNC_PTR_CHECK(controller_basic.public_elem.constructor,return X_False);
    if(controller_basic.public_elem.constructor(&controller_basic) == X_False) {return X_False;}
    
    return X_True;
}

X_Void ClassRun(uint32_t ms)
{
    ms = ms;
    if(THIS(controller_basic).key_subject != X_Null) {THIS(controller_basic).key_subject();}
}
