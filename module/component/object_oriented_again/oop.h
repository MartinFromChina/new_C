#ifndef __OOP_H
#define __OOP_H


#define CALL_FUNC_PTR(ptr, ...) ((ptr) != COMMON_MULL_FUNC ? (ptr)(__VA_ARGS__) : 0) 

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "oop_common.h"
#include "oop_func_overload.h"
#include "oop_template_func.h"

/***************************************************************class define*********************************************************************/
/*********************************************************W_CLASS_DEF*************************************************************************/
/******************************************************new oop****************************************************************************/
#define     __CLASS_DEF_3(__name,public_elements,protect_elements,private_elements)             typedef struct WOOP_CONCAT_2(__name,_pub){          \
                                                                                                        public_elements                             \
                                                                                                    }WOOP_CONCAT_2(__name,_pub);                    \
                                                                                                typedef struct WOOP_CONCAT_2(__name,_pri){          \
                                                                                                        private_elements                             \
                                                                                                    }WOOP_CONCAT_2(__name,_pri);                    \
                                                                                                typedef struct WOOP_CONCAT_2(__name,_pro){          \
                                                                                                        protect_elements                             \
                                                                                                    }WOOP_CONCAT_2(__name,_pro);                    \
                                                                                                typedef struct WOOP_CONCAT_2(__name,_inherit){      \
                                                                                                        WOOP_CONCAT_2(__name,_pub) public_elem;     \
                                                                                                        WOOP_CONCAT_2(__name,_pro) protect_elem;    \
                                                                                                    }WOOP_CONCAT_2(__name,_inherit);                \
                                                                                                 typedef struct __name{                                         \
                                                                                                        WOOP_CONCAT_2(__name,_pub) public_elem;                     \
                                                                                                        uint8_t invisible_pro[sizeof(struct WOOP_CONCAT_2(__name,_pro))]     \
                                                                                                                __attribute__((aligned(                                 \
                                                                                                                __alignof__(struct WOOP_CONCAT_2(__name,_pro))                 \
                                                                                                        )));                                                            \
                                                                                                        uint8_t invisible_pri[sizeof(struct WOOP_CONCAT_2(__name,_pri))]       \
                                                                                                                __attribute__((aligned(                                 \
                                                                                                                __alignof__(struct WOOP_CONCAT_2(__name,_pri))                 \
                                                                                                        )));                                                            \
                                                                                                    }__name                                       


#define __CLASS_DEF_2(__name,public_elements,private_elements)      typedef struct WOOP_CONCAT_2(__name,_pub){          \
                                                                                public_elements                         \
                                                                            }WOOP_CONCAT_2(__name,_pub);                \
                                                                     typedef struct WOOP_CONCAT_2(__name,_inherit){     \
                                                                        public_elements                                 \
                                                                    }WOOP_CONCAT_2(__name,_inherit);                        \
                                                                    typedef struct WOOP_CONCAT_2(__name,_pri){                  \
                                                                        private_elements                                            \
                                                                    }WOOP_CONCAT_2(__name,_pri);                                        \
                                                                    typedef struct __name{                                                  \
                                                                                WOOP_CONCAT_2(__name,_pub) public_elem;                         \
                                                                                uint8_t invisible_pri[sizeof(struct WOOP_CONCAT_2(__name,_pri))]     \
                                                                                        __attribute__((aligned(                                     \
                                                                                            __alignof__(struct WOOP_CONCAT_2(__name,_pri))          \
                                                                                        )));                                                        \
                                                                            }__name   

#define __CLASS_DEF_1(__name,public_elements)                 typedef struct WOOP_CONCAT_2(__name,_pub){         \
                                                                        public_elements                          \
                                                                    }WOOP_CONCAT_2(__name,_pub);                 \
                                                              typedef struct WOOP_CONCAT_2(__name,_inherit){     \
                                                                        public_elements                          \
                                                                    }WOOP_CONCAT_2(__name,_inherit);             \
                                                              typedef struct __name{                             \
                                                                        WOOP_CONCAT_2(__name,_pub) public_elem;  \
                                                                    }__name                       

#define CLASS_DEF_1(__name,public_elements)                                         __CLASS_DEF_1(__name,public_elements)
#define CLASS_DEF_2(__name,public_elements,private_elements)                        __CLASS_DEF_2(__name,public_elements,private_elements)
#define CLASS_DEF_3(__name,public_elements,protect_elements,private_elements)       __CLASS_DEF_3(__name,public_elements,protect_elements,private_elements)

/**************************************************************************************************
假设 PROTECT_MEMBER 使用频率不高，将他放在最后， CLASS_DEF(__name,...)后面的可变参数数量可以是1,2，或3个。
依次对应：public_member,    private_member,     protect_member

务实的角度说，私有成员变量不太需要是函数指针或者结构体指针，因此也不太需要const修饰
***************************************************************************************************/
#define NO_PRIVATE_MEMBER                 uint8_t place_holder
#define NO_PRIVATE_VALUE                  0

#define CLASS_DEF(__name,...)             WOOP_CONCAT_2(CLASS_DEF_,__WOOP_VA_NUM_ARGS(__VA_ARGS__))(__name,__VA_ARGS__)

/**********************************************************class inherit*******************************************************************/
#define INHERIT_CLASS(__name)                         WOOP_CONCAT_2(__name,_inherit)  * const WOOP_CONCAT_2(p_parent_,__name)
#define CLASS_INHERIT(__name,__obj)                   (WOOP_CONCAT_2(__name,_inherit) *)(&__obj.public_elem)
/**********************************************************class combinate*****************************************************************/
#define COMBINATE_CLASS(__name)                       __name  WOOP_CONCAT_2(partner_,__name)
/******************************************************************************************************************************************/
#undef  THIS
#define THIS(__obj)                              __obj.public_elem
#define P_THIS(__p_obj)                              __p_obj ->public_elem
// use the marco below after the __obj_ptr is checked 


// use marco IMPLEMENT_OF_##__name to enable PRIVATE_INTERNAL and PROTECT_INTERNAL
#define __PROTECT_INTERNAL(__name,__obj_ptr,__ptr)                        \
    WOOP_CONCAT_2(__name,_pro) * __ptr =                                \
        (WOOP_CONCAT_2(__name,_pro) *)((__obj_ptr) -> invisible_pro)

#define PROTECT_INTERNAL_1(__name,__obj_ptr,__ptr)      __PROTECT_INTERNAL(__name,__obj_ptr,__ptr)
#define PROTECT_INTERNAL(__name,__obj_ptr,__ptr)        WOOP_CONCAT_2(PROTECT_INTERNAL_,WOOP_CONCAT_2(IMPLEMENT_OF_,__name))(__name,__obj_ptr,__ptr)




#define __PRIVATE_INTERNAL(__name,__obj_ptr,__ptr)                        \
    WOOP_CONCAT_2(__name,_pri) * __ptr =                                \
        (WOOP_CONCAT_2(__name,_pri) *)((__obj_ptr) -> invisible_pri)

#define PRIVATE_INTERNAL_1(__name,__obj_ptr,__ptr)      __PRIVATE_INTERNAL(__name,__obj_ptr,__ptr)
#define PRIVATE_INTERNAL(__name,__obj_ptr,__ptr)        WOOP_CONCAT_2(PRIVATE_INTERNAL_,WOOP_CONCAT_2(IMPLEMENT_OF_,__name))(__name,__obj_ptr,__ptr)
/******************************************************************************************************************************************/

/*

CLASS_DEF(name
	,// public

    ,// protect

	,// private
        NO_PRIVATE_MEMBER;
	
);


static name name1 ={
    // public
	{

	},
	// protect  :init protect member in construct func 
    {0},
    // private  :init private member in construct func
    {0},
};





CLASS_DEF(example1
	,// public
        uint8_t public_member;
);

CLASS_DEF(example2
	,// public
        uint8_t public_member;
    ,// private
        uint8_t private_member;
);

CLASS_DEF(example3
	,// public
        uint8_t public_member;
    ,// protect
        uint8_t protect_member;
	,// private
        uint8_t private_member;
);

CLASS_DEF(example4
	,// public
        uint8_t public_member;
    ,// protect
        uint8_t protect_member;
	,// private
        NO_PRIVATE_MEMBER;
);


*/

#ifdef __cplusplus
		}
#endif

#endif

