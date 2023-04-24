DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT
TARGET = test

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/loop_scheduler.o  ./obj/InsertLogDebug.o ./obj/AppCommon.o  ./obj/loop_scheduler_test.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/loop_scheduler.o : ../loop_scheduler.c  ../loop_scheduler.h 
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c ../loop_scheduler.c -o $@
       
./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/loop_scheduler_test.o : loop_scheduler_test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c loop_scheduler_test.cc -o $@

clean:
	rm 		./obj/loop_scheduler.o ./obj/loop_scheduler_test.o ./obj/InsertLogDebug.o  ./obj/AppCommon.o 

