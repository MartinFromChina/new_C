DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = DG

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : DG_test.o DG_terminal.o  hal.o InsertLogDebug.o terminal1.o terminal2.o hal.o terminal3.o DG_terminal_interconnect.o  DG_protocol.o  \
					node_interconnect.o StateMachine.o priority_queues.o bit_operation.o test_data.o loop_queues.o ProtocolRecv.o test_common.o
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

loop_queues.o:../../../../module/common/loopqueue/loop_queues.c   ../../../../module/common/loopqueue/loop_queues.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/loopqueue/loop_queues.c

ProtocolRecv.o:../../../../module/customize/ProtocolRecv/ProtocolRecv.c   ../../../../module/customize/ProtocolRecv/ProtocolRecv.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/customize/ProtocolRecv/ProtocolRecv.c
           
StateMachine.o:../../../../module/common/StateMachine/StateMachine.c   ../../../../module/common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/StateMachine/StateMachine.c
            
DG_terminal.o :../DG_terminal.c   ../DG_terminal.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_terminal.c
            
terminal1.o :../terminal1.c   ../terminal1.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal1.c
            
node_interconnect.o :../../node_interconnect/node_interconnect.c   ../../node_interconnect/node_interconnect.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../node_interconnect/node_interconnect.c

terminal2.o :../terminal2.c   ../terminal2.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal2.c

terminal3.o :../terminal3.c   ../terminal3.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal3.c

hal.o: ../hal.c   ../hal.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../hal.c
            
DG_terminal_interconnect.o: ../DG_terminal_interconnect.c    ../DG_terminal_interconnect.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_terminal_interconnect.c

DG_protocol.o: ../DG_protocol.c    ../DG_protocol.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_protocol.c
            
DG_test.o : DG_test.cc # ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c DG_test.cc

test_data.o : test_data.cc # ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_data.cc

test_common.o : test_common.cc test_common.h# ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_common.cc
						
clean:
	rm DG_test.o DG_terminal.o  hal.o InsertLogDebug.o terminal1.o terminal2.o terminal3.o DG_terminal_interconnect.o  DG_protocol.o  \
	node_interconnect.o StateMachine.o priority_queues.o bit_operation.o test_data.o loop_queues.o ProtocolRecv.o test_common.o