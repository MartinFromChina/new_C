DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET2 = RGB_Display

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

OBJ = test_main.o  StateMachine.o loop_queues.o  test_11.o   
# SRC_FILES = $(filter-out ./obj/Motion_test.o, $(OBJ))


$(TARGET2) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $^ $(DLIBS)

loop_queues.o : ../../../../module/common/loopqueue/loop_queues.c ../../../../module/common/loopqueue/loop_queues.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c ../../../../module/common/loopqueue/loop_queues.c -o $@

test_11.o : ../../../../module/common/StateMachine/test_11.c ../../../../module/common/StateMachine/test_11.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
     -fPIC -c ../../../../module/common/StateMachine/test_11.c -o $@ 
      
StateMachine.o : ../../../../module/common/StateMachine/StateMachine.c ../../../../module/common/StateMachine/StateMachine.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
     -fPIC -c ../../../../module/common/StateMachine/StateMachine.c -o $@  

test_main.o : test_main.cc
	$(CXX) $(CXXFLAGS) -I../../../../module/common   -I../../../../module/common/StateMachine \
     -fPIC -c test_main.cc -o $@   


clean:
	rm 		StateMachine.o  main.o RGB_Display loop_queues.o test_11.o
	

