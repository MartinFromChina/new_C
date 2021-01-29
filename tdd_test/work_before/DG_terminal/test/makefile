DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = DG

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/DG_test.o 					./obj/DG_terminal.o  	./obj/hal.o 	./obj/InsertLogDebug.o 	\
			./obj/terminal1.o 					./obj/terminal2.o 	 	./obj/terminal3.o 						\
			./obj/DG_terminal_interconnect.o  	./obj/DG_protocol.o  											\
			./obj/node_interconnect.o 			./obj/StateMachine.o 	./obj/priority_queues.o 				\
			./obj/bit_operation.o 				./obj/test_data.o 		./obj/loop_queues.o 					\
			./obj/ProtocolRecv.o 				./obj/test_common.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/bit_operation.o : ../../../../module/common/Math/bit_operation.c ../../../../module/common/Math/bit_operation.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
           -c ../../../../module/common/Math/bit_operation.c -o $@
            
./obj/priority_queues.o:../../../../module/common/priorityqueue/priority_queues.c   ../../../../module/common/priorityqueue/priority_queues.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
          -c ../../../../module/common/priorityqueue/priority_queues.c -o $@

./obj/InsertLogDebug.o:../../../../module/common/InsertLog/InsertLogDebug.c   ../../../../module/common/InsertLog/InsertLogDebug.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/loop_queues.o:../../../../module/common/loopqueue/loop_queues.c   ../../../../module/common/loopqueue/loop_queues.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/loopqueue/loop_queues.c -o $@

./obj/ProtocolRecv.o:../../../../module/customize/ProtocolRecv/ProtocolRecv.c   ../../../../module/customize/ProtocolRecv/ProtocolRecv.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/customize/ProtocolRecv/ProtocolRecv.c -o $@
           
./obj/StateMachine.o:../../../../module/common/StateMachine/StateMachine.c   ../../../../module/common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
           -c ../../../../module/common/StateMachine/StateMachine.c -o $@
            
./obj/DG_terminal.o :../DG_terminal.c   ../DG_terminal.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_terminal.c -o $@
            
./obj/terminal1.o :../terminal1.c   ../terminal1.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal1.c -o $@
            
./obj/node_interconnect.o :../../node_interconnect/node_interconnect.c   ../../node_interconnect/node_interconnect.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../node_interconnect/node_interconnect.c -o $@

./obj/terminal2.o :../terminal2.c   ../terminal2.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal2.c -o $@

./obj/terminal3.o :../terminal3.c   ../terminal3.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../terminal3.c -o $@

./obj/hal.o: ../hal.c   ../hal.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../hal.c -o $@
            
./obj/DG_terminal_interconnect.o: ../DG_terminal_interconnect.c    ../DG_terminal_interconnect.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_terminal_interconnect.c -o $@

./obj/DG_protocol.o: ../DG_protocol.c    ../DG_protocol.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../DG_protocol.c -o $@
            
./obj/DG_test.o : DG_test.cc # ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c DG_test.cc -o $@

./obj/test_data.o : test_data.cc # ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_data.cc -o $@

./obj/test_common.o : test_common.cc test_common.h# ../node_interconnect.h  #  ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_common.cc -o $@
						
clean:
	rm 		./obj/DG_test.o 					./obj/DG_terminal.o  	./obj/hal.o 	./obj/InsertLogDebug.o 	\
			./obj/terminal1.o 					./obj/terminal2.o 	 	./obj/terminal3.o 						\
			./obj/DG_terminal_interconnect.o  	./obj/DG_protocol.o  											\
			./obj/node_interconnect.o 			./obj/StateMachine.o 	./obj/priority_queues.o 				\
			./obj/bit_operation.o 				./obj/test_data.o 		./obj/loop_queues.o 					\
			./obj/ProtocolRecv.o 				./obj/test_common.o