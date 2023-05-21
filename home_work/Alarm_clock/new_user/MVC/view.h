#ifndef __VIEW_H
#define __VIEW_H


#ifdef __cplusplus
 extern "C"{
#endif

#include "../../../../../module/component/object_oriented_again/oop.h"
#include "controller.h"

typedef   struct  view view; 

CLASS_DEF(view 
     ,// public 
     X_Boolean (* const constructor)(view * p_this);
     const key_updata key_evt_updata;
     X_Void (* const runner)(uint32_t ms);
     const view_updata view_evt_updata;
     ,// private
     uint8_t  hour;
     uint8_t  minute;
     uint8_t  sec;
);
     
#ifdef __cplusplus
    }
#endif

#endif

