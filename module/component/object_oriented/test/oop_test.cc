#include <iostream>
#include "tdd_common.h"

#include "../w_oop.h"
#include "LoraCommuProtocol.h"
#include"../../../common/AppCommon.h"
using namespace std;

/* 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint8_t call_cnt_buf[4],base_value = 0;

typedef X_Void (* todo0)(X_Void);
typedef X_Void (* todo1)(uint8_t a);
typedef X_Boolean (* todo2)(uint8_t a,sLoraDataHeader * p_header); 
typedef X_Void (* todo3)(uint8_t a,X_Boolean isOK,uint32_t b);

#include <stdio.h>
static X_Void TestAction0(X_Void)
{
	call_cnt_buf[0] ++;
    //printf("TestAction0 execu :base_value %d vs %d \r\n",base_value,call_cnt_buf[0]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[0]);
	EXPECT_EQ(base_value,call_cnt_buf[1]);
	EXPECT_EQ(base_value,call_cnt_buf[2]);
	EXPECT_EQ(base_value,call_cnt_buf[3]);
}
static X_Void TestAction1(uint8_t a)
{
	call_cnt_buf[1] ++;
	a = a ;
    //printf("a = %d :base_value %d vs %d \r\n",a,base_value,call_cnt_buf[1]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[0]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[1]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[2]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[3]);
	base_value ++;
}
static X_Boolean TestAction2(uint8_t a,sLoraDataHeader *p_header)
{
	call_cnt_buf[2] ++;
    //printf("a = %d header %2x\r\n",a,p_header ->header[0]);
	a = a ; p_header = p_header;
	EXPECT_EQ(base_value + 1,call_cnt_buf[0]);
	EXPECT_EQ(base_value,call_cnt_buf[1]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[2]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[3]);

	return X_True;
}
static X_Void TestAction3(uint8_t a,X_Boolean isOK,uint32_t b)
{
	call_cnt_buf[3] ++;
	a = a; b = b; isOK = isOK;
	//printf("a = %d isOK %2x ; b is %d \r\n",a,isOK,b);
	EXPECT_EQ(base_value + 1,call_cnt_buf[0]);
	EXPECT_EQ(base_value,call_cnt_buf[1]);
	EXPECT_EQ(base_value,call_cnt_buf[2]);
	EXPECT_EQ(base_value + 1,call_cnt_buf[3]);
	
}

TEST(func,overloadinbg)
{
	sLoraDataHeader test_header;

	test_header.header[0] = 0x55;

	byteBufInit(call_cnt_buf,4,0);
	base_value = 0;

	typedef struct 
	{
		X_Void (* hello)(uint8_t i);	
	}s_Func_todo100;

	typedef struct 
	{
		INSERT_OL_FUNC(todo,X_Void);
		INSERT_OL_FUNC(todo,X_Void,
						uint8_t a);
		INSERT_OL_FUNC(todo,X_Boolean,
						uint8_t a,sLoraDataHeader * p_header);	
		INSERT_OL_FUNC(todo,X_Void,
						uint8_t a,X_Boolean isOK,uint32_t b);			
	}s_Func_todo;

	s_Func_todo todo = {
		TestAction0,
		TestAction1,TestAction2,TestAction3
	};

	W_OLF_CALL(todo);
	W_OLF_CALL(todo,100,X_True,65535);
	W_OLF_CALL(todo,25,&test_header);
	W_OLF_CALL(todo,253);

	typedef struct 
	{
		sLoraDataHeader head;
		s_Func_todo     todo;
	}s_Combinate;
	
	s_Combinate com = {
		{
			0x55,0xaa,0x12345678,0x76543210,0x11223344,100,25,
		},
		{
			TestAction0,
			TestAction1,
			TestAction2,
			TestAction3,
		},
	};
	
	com.W_OLF_CALL(todo);
	com.W_OLF_CALL(todo,100,X_True,65535);
	com.W_OLF_CALL(todo,25,&test_header);
	com.W_OLF_CALL(todo,253);
/********************************full marco used *************************************************/
	W_OLF_DEF(todo_new,
		INSERT_OL_FUNC(todo_new,X_Void);
		INSERT_OL_FUNC(todo_new,X_Void,
						uint8_t a);
		INSERT_OL_FUNC(todo_new,X_Boolean,
						uint8_t a,sLoraDataHeader * p_header);	
		INSERT_OL_FUNC(todo_new,X_Void,
						uint8_t a,X_Boolean isOK,uint32_t b);		
	);

	W_OLF_INIT(todo_new,
		TestAction0,
		TestAction1,
		TestAction2,
		TestAction3,
	);

	W_OLF_CALL(todo_new);
	W_OLF_CALL(todo_new,100,X_True,65535);
	W_OLF_CALL(todo_new,25,&test_header);
	W_OLF_CALL(todo_new,253);
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	typedef struct 
	{
		sLoraDataHeader head;
		s_todo_new      todo_new;
	}s_Combinate_new;
	
	s_Combinate_new com_new = {
		{
			0x55,0xaa,0x12345678,0x76543210,0x11223344,100,25,
		},
		todo_new,
	};	
	com_new.W_OLF_CALL(todo_new);
	com_new.W_OLF_CALL(todo_new,100,X_True,65535);
	com_new.W_OLF_CALL(todo_new,25,&test_header);
	com_new.W_OLF_CALL(todo_new,253);

	s_Combinate_new * p_com = &com_new;
	p_com ->W_OLF_CALL(todo_new);
	p_com ->W_OLF_CALL(todo_new,100,X_True,65535);
	p_com ->W_OLF_CALL(todo_new,25,&test_header);
	p_com ->W_OLF_CALL(todo_new,253);
/************************************************************************************************/
};

/***********************

***************************/
/* 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static X_Void    fishing_method(X_Void)
{
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ we are fishing by the river  \r\n");
}
static X_Void    fishing_method2(X_Void)
{
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ john 's fishing is suck !!!  \r\n");
}
static X_Void    bark_method(X_Void)
{
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ wang wang \r\n");
}
static X_Void    playAction0(X_Void)
{
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ dudu stand up \r\n");
}
static X_Void    playAction1(uint8_t food)
{
	food = food;
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ dudu eat a lot food , and it feel very happy \r\n");
}
static X_Void    playAction2(uint8_t music,uint32_t toy_num)
{
	music = music;
	toy_num = toy_num;
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ dudu play with music , and its toys \r\n");
} 
static X_Boolean playAction3(uint8_t sleep,X_Boolean isOK,uint32_t outside)
{
	sleep = sleep;isOK = isOK;outside = outside;
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~ dudu sleep after going outside  \r\n");
	return X_False;
}

W_OLF_DEF(dog_play,
			INSERT_OL_FUNC(dog_play,X_Void);
			INSERT_OL_FUNC(dog_play,X_Void,
							uint8_t food);
			INSERT_OL_FUNC(dog_play,X_Boolean,
							uint8_t sleep,X_Boolean isOK,uint32_t outside);	
			INSERT_OL_FUNC(dog_play,X_Void,
							uint8_t music,uint32_t toy_num);		
		);
W_CLASS_DEF(dog
				, // public
					X_Void (*bark)(X_Void);
				);

W_CLASS_DEF(wife
			,// public
				uint32_t work;
			,// private
				uint8_t a_secret_lover;
			);

W_CLASS_DEF(father
			,// public
				uint32_t money;
				W_OLF_TYPE_DEF(dog_play) const dog_play; 
			,// private
				uint32_t another_women;
			);

W_CLASS_DEF(step_father
			,// public
				uint32_t money;
				X_Void (*fishing)(X_Void);
			);

W_CLASS_DEF(me
			,// public
				uint32_t work;
				W_CLASS_INHERIT(father);
				W_CLASS_INHERIT(step_father)[2];
				W_CLASS_COMBINATE(wife);
				W_CLASS_COMBINATE(dog);

			, // private
				uint8_t secret;
			);

const W_OLF_INIT(dog_play,
			playAction0,
			playAction1,
			playAction3,
			playAction2,
		);

	NEW_W_CLASS_PUBLIC(father,armStrong){
		"armStrong",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		1000000,
		dog_play,
	}NEW_W_CLASS_PRIVATE(father,armStrong){
		123,
	}NEW_W_CLASS_END(father,armStrong,armStrong);

	

	NEW_W_CLASS_PUBLIC(step_father,sam){
		"sam",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		1000,
		fishing_method,
	}NEW_W_CLASS_END(step_father,sam);

	NEW_W_CLASS_PUBLIC(step_father,john){
		"john",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		1000,
		fishing_method2,
	}NEW_W_CLASS_END(step_father,john);




	NEW_W_CLASS_PUBLIC(wife,my_wife){
		"Marry",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		8000,
	}NEW_W_CLASS_PRIVATE(wife,my_wife){
		255,
	}NEW_W_CLASS_END(wife,my_wife,my_wife);


	NEW_W_CLASS_PUBLIC(dog,Dudu){
		"Dudu",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		bark_method,
	}NEW_W_CLASS_END(dog,Dudu);



	NEW_W_CLASS_PUBLIC(me,myself){
		"bod",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		10000,   
		&armStrong, // dad 
		{&sam, &john,},//  
		{
			&my_wife,
			&my_wife_private,
		},
		{ 
			&Dudu,
		},
	}NEW_W_CLASS_PRIVATE(me,myself){
		1,
	}NEW_W_CLASS_END(me,myself,myself);

X_Boolean  TopInterface( s_all_class_base const *  p_base,uint32_t op_code)
{
	op_code = op_code;
	P_THIS_OF_CLASS_PUBLIC(me);
	printf("\r\n ---------- my name is %s \r\n",p_this->base.name);
	printf(" ---------- I earn %d dollor every month by my work \r\n",p_this->work);
	printf(" ---------- I have a secret %d \r\n",all_myself.private_elem->secret);

	printf("\r\n ---------- my dad name is %s , he leave %d dollars for me  \r\n"
		,p_this->p_parent_father->base.name
		,p_this->p_parent_father->money);

	printf("\r\n ---------- also I have a wife  %s , she earn %d dollars a month  \r\n"
		,p_this->partner_wife.public_elem->base.name
		,p_this->partner_wife.public_elem->work);
	printf(" ---------- and I know she has a secret lover %d \r\n",p_this->partner_wife.private_elem->a_secret_lover);
	
	printf("\r\n ----------  we also have a dog  %s , it going to bark  \r\n"
		,p_this->partner_dog.public_elem->base.name);

	p_this ->partner_dog.public_elem->bark();

	printf("\r\n ----------  my father have some special method to play with dog , I just learn from him , let me show you \r\n");

/**************************************
 * 这个用法有没有掌握的必要。。。。。
 * **********************************************/
/*
	father * const * p_father_class = &p_this ->public_elem.p_parent_father;
	(*p_father_class) ->W_OLF_CALL(dog_play,3,X_True,3);
	(*p_father_class) ->W_OLF_CALL(dog_play,2,2);
	(*p_father_class) ->W_OLF_CALL(dog_play,1);
	(*p_father_class) ->W_OLF_CALL(dog_play);
*/
/*
	WITH(const father,&p_this ->public_elem.p_parent_father)
	{
		father_270 ->W_OLF_CALL(dog_play,3,X_True,3);
		father_270 ->W_OLF_CALL(dog_play,2,2);
		father_270 ->W_OLF_CALL(dog_play,1);
		father_270 ->W_OLF_CALL(dog_play);
	}
*/
	CONST_WITH(father,&p_this ->p_parent_father,father)
	{
		C_THIS(father) ->W_OLF_CALL(dog_play,3,X_True,3);
		C_THIS(father) ->W_OLF_CALL(dog_play,2,2);
		C_THIS(father) ->W_OLF_CALL(dog_play,1);

		WITH(dog_all,&p_this ->partner_dog,dog)
		{
			C_THIS(dog).public_elem->bark();
		}

		C_THIS(father) ->W_OLF_CALL(dog_play);
	}

	// 关于嵌套的for 循环临时变量重名 ： https://www.codenong.com/2393458/
	// C ++编译器将此视为有效，因为第二个代码的范围仅在{}括号内
	// 在大多数C语言中，这是非法的。它是合法的C ++声明，因此如果使用C ++编译器编译C，则可以接受： 
	CONST_WITH(step_father,&p_this ->p_parent_step_father[0],step_father1)
	{
		printf("\r\n ----------  Now I am learning fishing from my stpe_father %s \r\n",C_THIS(step_father1) ->base.name);
		C_THIS(step_father1) ->fishing();
		CONST_WITH(step_father,&p_this ->p_parent_step_father[1],step_father2)
		{
			printf("\r\n ----------  the other day  I am learning fishing from my stpe_father2 %s \r\n",C_THIS(step_father2) ->base.name);
			C_THIS(step_father2) ->fishing();//p_this ->public_elem.p_parent_step_father[1] ->fishing();
		}
		//printf("\r\n ----------  Now I am learning fishing from my stpe_father %s \r\n",CONST_THIS(step_father1) ->base.name);
		//C_THIS(step_father1) ->fishing();

	}

	

	

	return X_True;
}


TEST(class,inherit_combinate)
{
	
	TopInterface(&myself.base,3);
}
/* 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef X_Void (*p_fly_func)(X_Void);

X_Void FlayNoWay(X_Void)
{
	printf("I can't fly \r\n \r\n");
}
X_Void FlayWithARocket(X_Void)
{
	printf("I am flying with a rocket \r\n \r\n");
}

typedef X_Void (* p_setFlyBehaviour)(s_all_class_base const *p_base,p_fly_func new_behaviour,uint32_t i);

W_CLASS_DEF(Duck
			, // public 
				p_fly_func FlyBehaviour;
				p_setFlyBehaviour set_fly_method;
			,// private
				uint8_t sercet;
);

W_CLASS_DEF(Baby_Duck
			,  // public 
				p_fly_func FlyBehaviour;
				W_CLASS_INHERIT(Duck);
			);

typedef struct s_about_fly
{
	s_all_class_base const base ;
	p_fly_func        FlyBehaviour;
	///// p_setFlyBehaviour set_fly_method;
}s_about_fly;

X_Void setFlyMethod(s_all_class_base const *p_base,p_fly_func new_behaviour,uint32_t i)
{
	i = i;
	s_about_fly *p_ext = (s_about_fly *)p_base;
	p_ext ->FlyBehaviour = new_behaviour;
}

static NEW_W_CLASS_PUBLIC(Duck,Duck_basic){
	"Duck_basic",
	NO_CONSTRUCTOR,
	NO_DESTRUCTOR,
	FlayNoWay,
	setFlyMethod,
}NEW_W_CLASS_PRIVATE(Duck,Duck_basic){
		0,
}NEW_W_CLASS_END(Duck,Duck_basic,Duck_basic);

static NEW_W_CLASS_PUBLIC(Baby_Duck,little_red_duck){
		"little_red_duck",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		FlayNoWay,
		&Duck_basic,
	}NEW_W_CLASS_END(Baby_Duck,little_red_duck);

TEST(class,common_interface)
{
	WITH(Baby_Duck,&little_red_duck,little_red)
	{
		C_THIS(little_red).FlyBehaviour();
		C_THIS(little_red).p_parent_Duck ->set_fly_method(&little_red_duck.base,FlayWithARocket,0);
		C_THIS(little_red).FlyBehaviour();
	}

}
/* 4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef X_Void (*heat)(X_Void);
typedef X_Void (*prepare_stuff)(X_Void);
typedef X_Void (*brew)(X_Void);
typedef X_Void (*clean)(X_Void);


TEMPLATE_METHOD_DEFINE(make_hot_drinks,heat,prepare_stuff,brew,clean){

        if(T_FUNC(heat) != T_FUNC_NULL(heat))  {T_FUNC(heat)();}
		if(T_FUNC(prepare_stuff) != T_FUNC_NULL(prepare_stuff))  {T_FUNC(prepare_stuff)();}
		if(T_FUNC(brew) != T_FUNC_NULL(brew))  {T_FUNC(brew)();}
		if(T_FUNC(clean) != T_FUNC_NULL(clean))  {T_FUNC(clean)();}

}TEMPLATE_METHOD_END

TEMPLATE_METHOD_TYPE_DEFINE(make_hot_drinks);

static X_Void mother_heat(X_Void)
{
	printf(" 1 heat water  \r\n");
}
static X_Void mother_prepare_stuff(X_Void)
{
	printf(" 2 prepare potato , watermelon  \r\n");
}
static X_Void mother_brew(X_Void)
{
	printf(" 3 draw the hot water into the potato  \r\n");
}
static X_Void mother_clean(X_Void)
{
	printf(" 4 clean the  kitchen \r\n");
}

static X_Void girl_heat(X_Void)
{
	printf(" 1 ~~~~~~~ no need hot water  \r\n");
}
static X_Void girl_prepare_stuff(X_Void)
{
	printf(" 2 ~~~~~~~ find milk and sugar  \r\n");
}
static X_Void girl_brew(X_Void)
{
	printf(" 3 ~~~~~~~ mixup   \r\n");
}
static X_Void girl_clean(X_Void)
{
	printf(" 4 ~~~~~~~ just clean the table  \r\n");
}

W_CLASS_DEF(mother
			, // public
				TEMPLATE_METHOD_TYPE(make_hot_drinks) make_soup;
			 	make_hot_drinks make_funcs;
			 );

W_CLASS_DEF(girl
			, // public
				W_CLASS_INHERIT(mother);
			 	make_hot_drinks make_funcs;
			 );


static NEW_W_CLASS_PUBLIC(mother,my_mother){
		{
			"penny",
			NO_CONSTRUCTOR,
			NO_DESTRUCTOR,
		},
		TEMPLATE_METHOD(make_hot_drinks),
		{
			mother_heat,
			(prepare_stuff)0,//mother_prepare_stuff,
			mother_brew,
			mother_clean,
		},
}NEW_W_CLASS_END(mother,my_mother);

static NEW_W_CLASS_PUBLIC(girl,my_sister){
		{
		"julia",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		},
		&my_mother,
		{
			(heat)0,//girl_heat,
			girl_prepare_stuff,//(prepare_stuff)0,//girl_prepare_stuff,
			girl_brew,//(brew)0,//girl_brew,
			girl_clean,
		},
	
}NEW_W_CLASS_END(girl,my_sister);

TEST(mode,template_method)
{
	
	WITH(mother,&my_mother,mom)
	{
		C_THIS(mom).make_soup(&this_mom ->make_funcs,(make_hot_drinks *)0);
		WITH(const girl,&my_sister,julia)
		{
			C_THIS(julia).p_parent_mother ->make_soup(&this_mom ->make_funcs,&this_julia ->make_funcs);
		}
	}
	
}


//////////TEST(mode,template_method_and_func_overlaod) // necessary  ? 
//////////{

//////////}
/* 5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef uint32_t (*get_cost)(s_all_class_base const * p_base);

W_CLASS_DEF(basic_cost
			,// public 
				get_cost 						get_cost_method;
			 	basic_cost 					* p_decorated;
				uint32_t (*get_self_cost)(X_Void);
			);



typedef struct 
{
	s_all_class_base  	const base;
	get_cost 			get_cost_method;
	basic_cost 			* p_decorated;
	uint32_t (*get_self_cost)(X_Void);
}cost_common_all;


W_CLASS_DEF(new_cost1
			,// public 
				get_cost 					get_cost_method;
			 	basic_cost 					* p_decorated;
				uint32_t (*get_self_cost)(X_Void);
			,//private 
				uint32_t addition_cost;     //  addition_cost in common maybe better 
			);

W_CLASS_DEF(new_cost2
			,// public 
				get_cost 					get_cost_method;
				basic_cost 					* p_decorated;
				uint32_t (*get_self_cost)(X_Void);
			,// private
				uint32_t addition_cost;
			);

//~~~~~~~~~~~~~~
static uint32_t get_basic_cost(s_all_class_base const * p_base)
{
	p_base = p_base;
	return 10000;
}

static uint32_t get_all_cost(s_all_class_base const * p_base) // recursive call ? 
{
	// printf(" called once \r\n");
	uint32_t pre_cost = 0;
	cost_common_all * p_common = (cost_common_all *)p_base;


	if(p_common ->p_decorated != (basic_cost *)0)
	{
		if(p_common ->p_decorated ->get_cost_method != (get_cost)0)
		{
			 pre_cost = p_common ->p_decorated ->get_cost_method(&p_common ->p_decorated->base);
		}
	}
	
	
	if(p_common ->get_self_cost != nullptr)
	{
		return (pre_cost + p_common ->get_self_cost());
	}
	else
	{
		return (pre_cost );
	}
	
}

static uint32_t GetSelfCost0(X_Void);

static NEW_W_CLASS_PUBLIC(basic_cost,b_cost){
		"basic_cost",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		get_basic_cost,
		(basic_cost*)0,
		GetSelfCost0,
}NEW_W_CLASS_END(basic_cost,b_cost);

static uint32_t GetSelfCost0(X_Void)
{
	return 0;
}

static uint32_t GetSelfCost1(X_Void);

static NEW_W_CLASS_PUBLIC(new_cost1,n_cost1){
		"new_cost1",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		get_all_cost,
		(basic_cost*)0,
		GetSelfCost1,
}NEW_W_CLASS_PRIVATE(new_cost1,n_cost1){
	1000,
}NEW_W_CLASS_END(new_cost1,n_cost1,n_cost1);

static uint32_t GetSelfCost1(X_Void)
{
	return all_n_cost1.private_elem->addition_cost;
}

static uint32_t GetSelfCost2(X_Void);
static NEW_W_CLASS_PUBLIC(new_cost1,n_cost2){
		"new_cost2",
		NO_CONSTRUCTOR,
		NO_DESTRUCTOR,
		get_all_cost,
		(basic_cost*)0,
		GetSelfCost2,
}NEW_W_CLASS_PRIVATE(new_cost1,n_cost2){
		100,
}NEW_W_CLASS_END(new_cost1,n_cost2,n_cost2);

static uint32_t GetSelfCost2(X_Void)
{
	return all_n_cost2.private_elem->addition_cost;
}
//~~~~~~~~~~~~~~

TEST(mode,Decorator)
{
	uint32_t cur_cost;

	cur_cost = b_cost.get_cost_method(&b_cost.base);
	printf("basic cost is %d \r\n",cur_cost);
	cur_cost = n_cost1.get_cost_method(&n_cost1.base);
	printf("new cost1 is %d \r\n",cur_cost);
	cur_cost = n_cost2.get_cost_method(&n_cost2.base);
	printf("new cost2 is %d \r\n",cur_cost);


	n_cost1.p_decorated = &b_cost;
	cur_cost = n_cost1.get_cost_method(&n_cost1.base);
	printf("new cost1 with basic cost is %d \r\n",cur_cost);

	n_cost2.p_decorated = (basic_cost*)&n_cost1;
	cur_cost = n_cost2.get_cost_method(&n_cost2.base);
	printf("new cost2 with new cost1 and basic cost is %d \r\n",cur_cost);
}
/* 6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* 7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

GTEST_API_ int main(int argc, char **argv) {
  cout<<"Running main() from stack_test.cc\n";

 // ::testing::GTEST_FLAG(filter) = "*class*:*mode*";

  testing::InitGoogleTest(&argc, argv);
  #if (WINDOWS_64_SYSTEM == 0)
  	return
  #else
	int value = 
  #endif
  RUN_ALL_TESTS();

#if (WINDOWS_64_SYSTEM!=0)
  value = value;
  while(1)
  {

  }
#endif
}



