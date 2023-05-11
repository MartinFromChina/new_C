#ifndef __OOP_FUNC_OVERLOAD_H
#define __OOP_FUNC_OVERLOAD_H

#ifdef __cplusplus
	extern "C" {
#else 
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
 
    #else
    #warning [caution !!! not support C99 ]
    #endif
#endif

#include "oop_common.h"

/*************************************weak function overloading 
 * weak means : the difference between those funcs is : the number of parameter , not the type of parameter 
 * *************************************************/ 

#define INSERT_OL_FUNC(__func_name,return_type,...)     return_type (*WOOP_CONCAT_3(__func_name,_,__WOOP_OLF_NUM_ARGS(__VA_ARGS__)))(__VA_ARGS__)   // return_type (*__func_name_"param_num"(...))

#define W_OLF_CALL(__func_name,...)  WOOP_CONCAT_3(__func_name.__func_name,_, __WOOP_VA_NUM_ARGS(__VA_ARGS__)(__VA_ARGS__))


#define W_OLF_DEF(__func_name,...)  typedef struct {   \
                                     __VA_ARGS__        \
                                    }WOOP_CONCAT_2(s_,__func_name)

#define W_OLF_INIT(__func_name,...) WOOP_CONCAT_2(s_,__func_name)  __func_name = {__VA_ARGS__}

#define W_OLF_TYPE_DEF(__func_name) WOOP_CONCAT_2(s_,__func_name)
/*********************** demo **************************
typedef X_Void (* todo0)(X_Void);
typedef X_Void (* todo1)(uint8_t a);
typedef X_Boolean (* todo2)(uint8_t a,sLoraDataHeader * p_header); 
typedef X_Void (* todo3)(uint8_t a,X_Boolean isOK,uint32_t b);

#include <stdio.h>
static X_Void TestAction0(X_Void)
{
    printf("TestAction0 execu \r\n");
}
static X_Void TestAction1(uint8_t a)
{
    printf("a = %d \r\n",a);
}
static X_Boolean TestAction2(uint8_t a,sLoraDataHeader *p_header)
{
    printf("a = %d header %2x\r\n",a,p_header ->header[0]);
	return X_True;
}
static X_Void TestAction3(uint8_t a,X_Boolean isOK,uint32_t b)
{
	printf("a = %d isOK %2x ; b is %d \r\n",a,isOK,b);
}

sLoraDataHeader test_header;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
W_OLF_DEF(overloading_func_class,
		INSERT_OL_FUNC(overloading_func_class,X_Void);
		INSERT_OL_FUNC(overloading_func_class,X_Void,
						uint8_t a);
		INSERT_OL_FUNC(overloading_func_class,X_Boolean,
						uint8_t a,sLoraDataHeader * p_header);	
		INSERT_OL_FUNC(overloading_func_class,X_Void,
						uint8_t a,X_Boolean isOK,uint32_t b);		
	);

	W_OLF_INIT(overloading_func_class,
		TestAction0,
		TestAction1,
		TestAction2,
		TestAction3,
	);

	W_OLF_CALL(overloading_func_class);
	W_OLF_CALL(overloading_func_class,100,X_True,65535);
	W_OLF_CALL(overloading_func_class,25,&test_header);
	W_OLF_CALL(overloading_func_class,253);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	typedef struct 
	{
		s_overloading_func_class      overloading_func_class;
	}s_Combinate_new;
	
	s_Combinate_new com_new = {
		overloading_func_class,
	};	
	com_new.W_OLF_CALL(overloading_func_class);
	com_new.W_OLF_CALL(overloading_func_class,100,X_True,65535);
	com_new.W_OLF_CALL(overloading_func_class,25,&test_header);
	com_new.W_OLF_CALL(overloading_func_class,253);

	s_Combinate_new * p_com = &com_new;
	p_com ->W_OLF_CALL(overloading_func_class);
	p_com ->W_OLF_CALL(overloading_func_class,100,X_True,65535);
	p_com ->W_OLF_CALL(overloading_func_class,25,&test_header);
	p_com ->W_OLF_CALL(overloading_func_class,253);
 * *************************************************/











/****************************************ignore the code below **********************************************/
//INSERT_OL_FUNC(todo,X_Void,1);
/*********************************************
 * W_OLF_CALL(haapy,1,2,3,todo)
 * 
INSERT_OL_FUNC(happy,X_Void);
INSERT_OL_FUNC(happy,X_Void,1);
INSERT_OL_FUNC(happy,X_Void,1,2);
INSERT_OL_FUNC(happy,X_Void,1,2,3);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5,6);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5,6,7);

INSERT_OL_FUNC(happy,X_Boolean,
                uint8_t i,uint32_t length,
                3,4,5,6,X_Boolean isOK);

INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5,6,7,8,9,10,11,12,13,14);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
INSERT_OL_FUNC(happy,X_Void,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
INSERT_OLFUNC(happy,X_Void,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17);
INSERT_OLFUNC(happy,X_Void,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18);

typedef X_Void (* todo0)(X_Void);
typedef X_Void (* todo1)(uint8_t a);
typedef X_Boolean (* todo2)(uint8_t a,sLoraDataHeader * p_header);
typedef X_Void (* todo3)(uint8_t a,X_Boolean isOK);

 * *****************************************/

#ifdef __cplusplus
		}
#endif

#endif
