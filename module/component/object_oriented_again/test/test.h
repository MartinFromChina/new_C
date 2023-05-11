#include "../oop.h"

CLASS_DEF(hello
        ,// public 
            uint8_t i;
        ,// private 
            uint8_t j;
    );

CLASS_DEF(world
        ,// public 
            uint8_t i;
        ,// protect
            uint8_t k;
        ,// private 
            NO_PRIVATE_MEMBER;
    );

CLASS_DEF(marry
        ,// public 
            uint8_t i;
    );

/*
    typedef struct 
    {
        uint8_t i;
    }s_public;

    typedef struct 
    {
        uint8_t j;
    }s_private;

    typedef struct 
    {
        s_public pub;
        s_private pri;
    }s_all;


    typedef struct 
    {
        s_all *p_all;
        uint8_t k;
    }s_another;

    s_all all1 = {{1},{2},};
    s_another another1 = {
        {&all1,},
        3,
    };

     s_another another2 = {
        {
            &all1.pub,
        },
        3,
    };
*/