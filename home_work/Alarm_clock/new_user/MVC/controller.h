#ifndef __CONTROLLER_H
#define __CONTROLLER_H


#ifdef __cplusplus
 extern "C"{
#endif

#include "../../../../module/component/object_oriented_again/oop.h"
#include  "LogDebugSwitch.h"
     
typedef enum
{
    ke_idle = 0,
    ke_click,
    ke_double_click,
    ke_long_push,
}e_basic_key_evt;

typedef  X_Void (* key_updata)(e_basic_key_evt evt,uint8_t key_id);

#define MAX_BUTTON_NUM       3
#define MAX_KEY_OBSERVER_NUM       2

typedef struct controller controller;

CLASS_DEF(controller 
     ,// public 
     X_Boolean (* const constructor)(controller * p_this);
     X_Void (* const key_evt)(uint8_t key_id,e_basic_key_evt evt,uint32_t ms);
     X_Void (* const key_subject)(X_Void);
     X_Boolean (* const key_observer_reg)(key_updata updata_func);
     ,// private 
     e_basic_key_evt key_evt_table[MAX_BUTTON_NUM];
     key_updata      key_observer[MAX_KEY_OBSERVER_NUM];
     uint8_t         observer_num;
);
     
#ifdef __cplusplus
    }
#endif

#endif

