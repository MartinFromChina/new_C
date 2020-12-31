DEFS = -DUSE_PRINT1 -DUSE_PRINT2=1 
CFLAGS += $(DEFS)  

test : test_main.c another.c another.h print1.c print2.c print3.c print1.h \
	print2.h print3.h	
	gcc $(CFLAGS) -o test test_main.c another.c print1.c print2.c print3.c

clean :
	rm test 
