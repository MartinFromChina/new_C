#include "../const_loop_scheduler.h"

#include "../refresh_task_tag.h"

X_do(hello);

#define __WITH_POINT_H(x) x##.h
#define WITH_POINT_H(x) __WITH_POINT_H(x)

#define __TO_STR(y)     #y
#define TO_STR(y)       __TO_STR(y)


#define INCLUDE(x)      inc##lude x


#define WITH_JING(z)   z


//WITH_JING(#2)


//INCLUDE(TO_STR(WITH_POINT_H(hello)))

WITH_JING(#INCLUDE(TO_STR(WITH_POINT_H(hello))))