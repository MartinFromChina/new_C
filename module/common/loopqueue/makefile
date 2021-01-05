DLIBS = ../../google_test/lib/libgmock.a
CXX = g++
INC = -I./common
TARGET = loop_queues

SRCFILES = loopqueue_test.cc 
#loop_queues.c

#DEPFILES := $(patsubst %.c,%.d,$(SRCFILES)) 

-include $(DEPFILES) 

#OBJS = $(SRCFILES: .c = .o)
OBJS = $(SRCFILES: .cc = .o)

CFLAGS = -pthread
#vpath %.h ../common

$(TARGET) : $(OBJS)
	echo TARGET:$@
	echo OBJECTS:$@
	$(CXX) $(CFLAGS) -o $@ $^ $(DLIBS)


#%.o: %.c makefile 
#	$(CXX) $(CFLAGS) -MMD -MP -o $@ -c $<
	
#loop_queues.h : ../KeilMDK.h ../CommonMarco.h
#loop_queues.o : loop_queues.c 
#	g++ -o loop_queues.c ../KeilMDK.h ../CommonMarco.h
#loopqueue_test.o : loopqueue_test.cc 
#	g++ -o loopqueue_test.cc loop_queues.h
clean:
	rm loop_queues



