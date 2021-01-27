DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = DG

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : node_test.o node_interconnect.o  node.o StateMachine.o InsertLogDebug.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

bit_operation.o : ../../../../module/common/Math/bit_operation.c ../../../../module/common/Math/bit_operation.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
            -c ../../../../module/common/Math/bit_operation.c
            
priority_queues.o:../../../../module/common/priorityqueue/priority_queues.c   ../../../../module/common/priorityqueue/priority_queues.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/priorityqueue/priority_queues.c

InsertLogDebug.o:../../../../module/common/InsertLog/InsertLogDebug.c   ../../../../module/common/InsertLog/InsertLogDebug.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/InsertLog/InsertLogDebug.c

node.o: ../node.c   ../node.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../node.c
loop_queues.o:../../../../module/common/loopqueue/loop_queues.c   ../../../../module/common/loopqueue/loop_queues.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/loopqueue/loop_queues.c
           
StateMachine.o:../../../../module/common/StateMachine/StateMachine.c   ../../../../module/common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/StateMachine/StateMachine.c
            
node_interconnect.o :../node_interconnect.c   ../node_interconnect.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../node_interconnect.c
            
node_test.o : node_test.cc  node_test.h ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c node_test.cc
						
clean:
	rm node_test.o node_interconnect.o  node.o StateMachine.o InsertLogDebug.o \
			node_trans_test.o priority_queues.o bit_operation.o loop_queues.o mul_trans_test.o trans_response_test.o