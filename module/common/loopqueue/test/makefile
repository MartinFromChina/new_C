DLIBS = ../../../google_test/lib/libgmock.a
TARGET = loop_queues

SRCFILES = loopqueue_test.cc 

OBJS = $(SRCFILES: .cc = .o) 
#loop_queues.o

CPPFLAGS += -isystem $(COMMON_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

all : $(TARGET)

clean:
	rm $(TARGET) loop_queues.o loopqueue_test.o

loop_queues.o : ../loop_queues.c ../loop_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
            -c ../loop_queues.c

loopqueue_test.o : loopqueue_test.cc tdd_common.h ../loop_queues.h ../../x_cross_platform.h ../../CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c loopqueue_test.cc
			

			
$(TARGET) : loopqueue_test.o loop_queues.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)





