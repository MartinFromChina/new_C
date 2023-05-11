SYS_SW_WIN64=1
variable_0=0
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DWINDOWS_64_SYSTEM=$(SYS_SW_WIN64) -DARM_COMPILER_PRESENT=1

ifeq ($(SYS_SW_WIN64),$(variable_0))
	DLIBS = ../../../google_test/lib/libgmock.a
else
	DLIBS = ../../../google_test/lib/libgmock_win64.a
	CXXFLAGS += -Wno-explicit  -Wno-deprecated-copy
endif

TARGET = mStack_out

SRCFILES = mStack.cc 

OBJS = $(SRCFILES: .cc = .o) 

CPPFLAGS += -isystem $(COMMON_DIR)
CXXFLAGS += $(DEFS)

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





