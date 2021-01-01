TARGET = test
SRCS = test_main.c another.c print1.c print2.c print3.c
OBJS = $(SRCS: .c = .o)

CC = gcc
CFLAGS = -Wall -std = gnu99

$(TARGET):$(OBJS)
	echo TARGET:$@
	echo OBJECTS:$@
	$(CC) -o $@ $^

clean :
	rm -rf $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<
