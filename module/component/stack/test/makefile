DLIBS = ../../../google_test/lib/libgmock.a
TARGET = mStack

SRCFILES = mStack.cc 

OBJS = $(SRCFILES: .cc = .o) 

CPPFLAGS += -isystem $(COMMON_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

all : $(TARGET)

clean:
	rm $(TARGET) mStack.o stack_test.o

mStack.o : ../mStack.c ../mStack.h ../../../common/x_cross_platform.h ../../../common/CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
            -c ../mStack.c

stack_test.o : stack_test.cc tdd_common.h ../mStack.h ../../../common/x_cross_platform.h ../../../common/CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c stack_test.cc
			

			
$(TARGET) : stack_test.o mStack.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)





