DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = node

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : node_test.o node_interconnect.o  node.o #StateMachine.o char_to_num.o loop_queues.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

#loop_queues.o : ../../../common/loopqueue/loop_queues.c   ../../../common/loopqueue/loop_queues.h
#	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
#           -c ../../../common/loopqueue/loop_queues.c

#char_to_num.o:../../../common/CharToNum/char_to_num.c   ../../../common/CharToNum/char_to_num.h
#	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
#            -c ../../../common/CharToNum/char_to_num.c

node.o: ../node.c   ../node.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../node.c

#StateMachine.o:../../../common/StateMachine/StateMachine.c   ../../../common/StateMachine/StateMachine.h 
#	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
#           -c ../../../common/StateMachine/StateMachine.c
            
node_interconnect.o :../node_interconnect.c   ../node_interconnect.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../node_interconnect.c
  
node_test.o : node_test.cc   #../ProtocolRecv.h    ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c node_test.cc
						
clean:node_test.o node_interconnect.o  node.o #StateMachine.o char_to_num.o loop_queues.otest_main.o ProtocolRecv.o StateMachine.o rw_by_line.o char_to_num.o loop_queues.o