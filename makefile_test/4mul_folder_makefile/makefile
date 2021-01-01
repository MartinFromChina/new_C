TARGET = test
SRCS = test_main.c another.c \
       ./print1/print1.c \
       ./print2/print2.c \
       ./print3/print3.c
OBJS = $(SRCS: .c = .o)

CC = gcc
CFLAGS = -Wall -std = gnu99

INC = -I./print1 -I./print2 -I./print3

$(TARGET):$(OBJS)
	echo TARGET:$@
	echo OBJECTS:$@
	$(CC) -o $@ $^

clean :
	rm -rf $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
