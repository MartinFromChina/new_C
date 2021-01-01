#include "another.h"
#include "print1.h"
#include "print2.h"
#include "print3.h"

int main()
{
	another_print();
#ifdef USE_PRINT1 
	print1();
#endif

#if (USE_PRINT2 == 1)
	print2();
#endif
	print3();
}
