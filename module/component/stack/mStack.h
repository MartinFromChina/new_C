#ifndef __MSTACK_H
#define __MSTACK_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "../../common/x_cross_platform.h"
#include "../../common/CommonMarco.h"

#define INVALID_STACK_NODE_NUM  (0xFFFF)

#define APP_STACK_DEF(__id,__deepth,__type_def,__type_size)    

#define STACK_PUSH(__id,__elem)   s_push(__id,(uint8_t *)&__elem,__id ->type_size)

#define STACK_POP

#define DOES_STACK_EMPTY

#define DOES_STACK_FULL

#define GET_STACK_USAGE

// 参考可变参数的队列实现
// https://mp.weixin.qq.com/s/XZZVhUgdydZ_7yUHMqR2mw 
// 将栈和队列以及优先级队列全部做成支持任意类型的模块。做完了静态分配再做动态内存分配




















#ifdef __cplusplus
		}
#endif

#endif