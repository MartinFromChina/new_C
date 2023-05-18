#ifndef __MODEL_H
#define __MODEL_H


#ifdef __cplusplus
 extern "C"{
#endif

#include "../../../../../module/component/object_oriented_again/oop.h"

typedef struct model model;
     
CLASS_DEF(model
     ,// public
        X_Boolean (* const constructor)(model * p_this);
        X_Void (* const runner)(uint32_t ms);
        X_Void (* const interface_next)(X_Void);
        X_Void (* const interface_back)(X_Void);
     ,// private
        uint16_t interface_id; 
);
     
#ifdef __cplusplus
    }
#endif

#endif

