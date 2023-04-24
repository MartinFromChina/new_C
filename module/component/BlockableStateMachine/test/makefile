SYS_SW_WIN64=0
variable_0=0
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT -DWINDOWS_64_SYSTEM=$(SYS_SW_WIN64)
TARGET = test

ifeq ($(SYS_SW_WIN64),$(variable_0))
	DLIBS = ../../../google_test/lib/libgmock.a
else
	DLIBS = ../../../google_test/lib/libgmock_win64.a
	CXXFLAGS += -Wno-explicit  -Wno-deprecated-copy
endif

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/b_StateMachine.o  ./obj/InsertLogDebug.o ./obj/AppCommon.o  ./obj/test.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)
       
./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/b_StateMachine.o : ../b_StateMachine.c tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../b_StateMachine.c -o $@

./obj/test.o : test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c test.cc -o $@

	   

clean:
	rm 		./obj/b_StateMachine.o  ./obj/InsertLogDebug.o  ./obj/AppCommon.o  ./obj/test.o

