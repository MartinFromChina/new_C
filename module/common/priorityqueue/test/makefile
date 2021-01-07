DLIBS = ../../../google_test/lib/libgmock.a
TARGET = prio_queues

SRCFILES = priorityqueue_test.cc 

OBJS = $(SRCFILES: .cc = .o) 

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

all : $(TARGET)

clean:
	rm $(TARGET) priority_queues.o priorityqueue_test.o

priority_queues.o : ../priority_queues.c ../priority_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
            -c ../priority_queues.c

priorityqueue_test.o : priorityqueue_test.cc tdd_common.h ../priority_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c priorityqueue_test.cc
			

			
$(TARGET) : priorityqueue_test.o priority_queues.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)





