DLIBS = ../../../google_test/lib/libgmock.a
TARGET = prio_queues
DEFS = -DUSE_TDD_PRIORITY_QUEUE_INTERNAL_TERST=0
SRCFILES = priorityqueue_test.cc 


OBJS = $(SRCFILES: .cc = .o) 

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
 CXXFLAGS += $(DEFS)

all : $(TARGET)

clean:
	rm $(TARGET) priority_queues.o priorityqueue_test.o bit_operation.o

bit_operation.o : ../../Math/bit_operation.c ../../Math/bit_operation.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
            -c ../../Math/bit_operation.c
            
priority_queues.o : ../priority_queues.c   ../../Math/bit_operation.c ../../Math/bit_operation.h   ../priority_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common   -I../../../common/Math \
            -c ../priority_queues.c

priorityqueue_test.o : priorityqueue_test.cc tdd_common.h ../priority_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c priorityqueue_test.cc
			

			
$(TARGET) : priorityqueue_test.o priority_queues.o bit_operation.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)





