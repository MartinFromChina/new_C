DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT
TARGET = test

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/thread_in_loop.o  ./obj/InsertLogDebug.o ./obj/AppCommon.o  ./obj/thread_in_loop_test.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/thread_in_loop.o : ../thread_in_loop.c  ../thread_in_loop.h 
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c ../thread_in_loop.c -o $@
       
./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/thread_in_loop_test.o : thread_in_loop_test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c thread_in_loop_test.cc -o $@

clean:
	rm 		./obj/thread_in_loop.o ./obj/thread_in_loop_test.o ./obj/InsertLogDebug.o  ./obj/AppCommon.o 

