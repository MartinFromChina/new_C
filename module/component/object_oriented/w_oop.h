#ifndef __W_OOP_H
#define __W_OOP_H


#define CALL_FUNC_PTR(ptr, ...) ((ptr) != COMMON_MULL_FUNC ? (ptr)(__VA_ARGS__) : 0) 

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "w_oop_common.h"
#include "w_oop_func_overload.h"
#include "w_oop_template_func.h"

/***************************************************************class define*****************************************************************************************************/

typedef struct s_all_class_base s_all_class_base;
typedef  X_Boolean  (*w_constructor)( s_all_class_base const * p_base);
typedef  X_Boolean  (*w_destructor)( s_all_class_base const * p_base);

#define NO_CONSTRUCTOR                                              ((w_constructor)0)
#define NO_DESTRUCTOR                                               ((w_destructor)0)

#define MAX_W_CLASS_NAME_LENGTH  30       // W means weak 
typedef struct s_all_class_base{
	uint8_t                 name[MAX_W_CLASS_NAME_LENGTH + 1]; // + 1 for empty space : ' '
    w_constructor           p_constructor;
    w_destructor            p_destructor;
}s_all_class_base;  
/*********************************************************W_CLASS_DEF*************************************************************************/
#define __W_CLASS_DEF_3(__class_name,public_elements,private_elements)  typedef struct __class_name{   \
                                                                        s_all_class_base const base ;  \
                                                                        public_elements    \
                                                                    }__class_name;          \
                                                                    typedef struct WOOP_CONCAT_2(__class_name,_private){    \
                                                                        private_elements    \
                                                                    }WOOP_CONCAT_2(__class_name,_private);                   \
                                                                    typedef struct WOOP_CONCAT_2(__class_name,_all)    {   \
                                                                        __class_name                           * public_elem;       \
                                                                        WOOP_CONCAT_2(__class_name,_private)   * private_elem;     \
                                                                    }WOOP_CONCAT_2(__class_name,_all)

#define __W_CLASS_DEF_2(__class_name,public_elements)                 typedef struct __class_name{   \
                                                                        s_all_class_base const base ;  \
                                                                        public_elements    \
                                                                    }__class_name;          \
                                                                    typedef struct WOOP_CONCAT_2(__class_name,_all)    {   \
                                                                        __class_name                           * public_elem;       \
                                                                    }WOOP_CONCAT_2(__class_name,_all)

#define __W_CLASS_DEF_1(__class_name)                               typedef struct __class_name{   \
                                                                        s_all_class_base const base ;  \
                                                                    }__class_name;          \
                                                                    typedef struct WOOP_CONCAT_2(__class_name,_all)    {   \
                                                                        __class_name                           * public_elem;       \
                                                                    }WOOP_CONCAT_2(__class_name,_all)

#define W_CLASS_DEF_0(__class_name)                                        __W_CLASS_DEF_1(__class_name)
#define W_CLASS_DEF_1(__class_name,public_elements)                        __W_CLASS_DEF_2(__class_name,public_elements)
#define W_CLASS_DEF_2(__class_name,public_elements,private_elements)       __W_CLASS_DEF_3(__class_name,public_elements,private_elements)

#define W_CLASS_DEF(__class_name,...)           WOOP_CONCAT_2(W_CLASS_DEF_,__WOOP_VA_NUM_ARGS(__VA_ARGS__))(__class_name,__VA_ARGS__)
/**********************************************************************************************************************************/
/**********************************************************************************************************************************/
#define __NEW_W_CLASS_END_1(__class_name,__name)                               ;   \
                                                                    static const WOOP_CONCAT_2(__class_name,_all) WOOP_CONCAT_2(all_,__name) = {   \
                                                                            &(__name),                                       \
                                                                    }

#define __NEW_W_CLASS_END_2(__class_name,__name,__p_name)                               ;   \
                                                                    static const WOOP_CONCAT_2(__class_name,_all) WOOP_CONCAT_2(all_,__name) = {   \
                                                                            &(__name),                                          \
                                                                            &WOOP_CONCAT_2(__p_name,_private),                                        \
                                                                    }
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~NEW_W_CLASS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define NEW_W_CLASS_PUBLIC(__class_name,__name)                             __class_name __name = 

#define NEW_W_CLASS_PRIVATE(__class_name,__p_name)                            ;     \
                                                                     static WOOP_CONCAT_2(__class_name,_private) WOOP_CONCAT_2(__p_name,_private) = 

#define NEW_W_CLASS_END(__class_name,...)               WOOP_CONCAT_2(__NEW_W_CLASS_END_,__WOOP_VA_NUM_ARGS(__VA_ARGS__))(__class_name,__VA_ARGS__)
                                                                     
// #define W_CLASS_DEF_END                           
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/**********************************************************************************************************************************/


/***************************************************************class inherit*****************************************************************************************************/
#define W_CLASS_INHERIT(__class_name)                            __class_name  * const WOOP_CONCAT_3(p_parent,_,__class_name)
/***************************************************************class combinate*****************************************************************************************************/
#define W_CLASS_COMBINATE(__class_name)                       WOOP_CONCAT_2(__class_name,_all)  WOOP_CONCAT_3(partner,_,__class_name)
/********************************************************************************************************************************************************************/
#define THIS(__obj_name)                         __obj_name.public_elements
#define PRIVATE(__obj_name)                     WOOP_CONCAT_2(__obj_name,_private)
#define ALL(__obj_name)                         WOOP_CONCAT_2(all_,__obj_name)

#define P_THIS_OF_CLASS_PUBLIC(__class_name)                         __class_name * p_this = (__class_name *)p_base
/********************************************************************************************************************************************************************/

#ifdef __cplusplus
		}
#endif

#endif

