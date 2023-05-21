#include "controller.h"
#include "model.h"
#include "view.h"


extern controller controller_basic;
extern model model1;
extern view view1;

X_Boolean ClassInit(X_Void)
{
    FUNC_PTR_CHECK(controller_basic.public_elem.constructor,return X_False);
    FUNC_PTR_CHECK(model1.public_elem.constructor,return X_False);
    FUNC_PTR_CHECK(view1.public_elem.constructor,return X_False);
    
    if(controller_basic.public_elem.constructor(&controller_basic) == X_False) {return X_False;}
    if(model1.public_elem.constructor(&model1) == X_False) {return X_False;}
    if(view1.public_elem.constructor(&view1) == X_False) {return X_False;}
    
    FUNC_PTR_CHECK(THIS(controller_basic).key_observer_reg,return X_False);
    THIS(controller_basic).key_observer_reg(THIS(model1).key_evt_updata);
    THIS(controller_basic).key_observer_reg(THIS(view1).key_evt_updata);
    
    FUNC_PTR_CHECK(THIS(model1).view_observer_reg,return X_False);
    THIS(model1).view_observer_reg(THIS(view1).view_evt_updata);
    
    FUNC_PTR_CHECK(THIS(model1).init,return X_False);
    THIS(model1).init();
    return X_True;
}

X_Void ClassRun(uint32_t ms)
{
    if(THIS(controller_basic).key_subject != X_Null) {THIS(controller_basic).key_subject();}
    if(THIS(model1).runner != X_Null) {THIS(model1).runner(ms);}
    if(THIS(view1).runner != X_Null) {THIS(view1).runner(ms);}
}
