
SYS_SW_WIN64=0
variable_0=0
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DWINDOWS_64_SYSTEM=$(SYS_SW_WIN64) -DARM_COMPILER_PRESENT=1

# echo " linux system"
# echo " win64 system"
# WINDOWS_64_SYSTEM,0
# dos2unix

ifeq ($(SYS_SW_WIN64),$(variable_0)))
	DLIBS = ../../../google_test/lib/libgmock.a
else
	DLIBS = ../../../google_test/lib/libgmock_win64.a
endif

TARGET = oop_test

SRCFILES = oop_test.cc 

OBJS = $(SRCFILES: .cc = .o) 

CPPFLAGS += -isystem $(COMMON_DIR)

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

all : $(TARGET)

clean:
	rm $(TARGET)  oop_test.o AppCommon.o

AppCommon.o: ../../../common/AppCommon.c  ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
            -c ../../../common/AppCommon.c

oop_test.o : oop_test.cc tdd_common.h  ../../../common/x_cross_platform.h ../../../common/CommonMarco.h
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c oop_test.cc
			

			
$(TARGET) : oop_test.o  AppCommon.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)





