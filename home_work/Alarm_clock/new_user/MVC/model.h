#ifndef __MODEL_H
#define __MODEL_H


#ifdef __cplusplus
 extern "C"{
#endif

#include "../../../../../module/component/object_oriented_again/oop.h"
#include "controller.h"
     
typedef struct model model;
     
CLASS_DEF(model
     ,// public
        X_Boolean (* const constructor)(model * p_this);
        const key_updata key_evt_updata;
        X_Void (* const runner)(uint32_t ms);
//////     ,// private
//////        uint16_t interface_id; 
);
     
#ifdef __cplusplus
    }
#endif

#endif

