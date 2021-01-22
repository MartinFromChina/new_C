DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = Recv

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : test_main.o ProtocolRecv.o StateMachine.o rw_by_line.o char_to_num.o loop_queues.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

loop_queues.o : ../../../common/loopqueue/loop_queues.c   ../../../common/loopqueue/loop_queues.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../../common/loopqueue/loop_queues.c

char_to_num.o:../../../common/CharToNum/char_to_num.c   ../../../common/CharToNum/char_to_num.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../../common/CharToNum/char_to_num.c

rw_by_line.o: ../../../common/FileOperation/rw_by_line.c   ../../../common/FileOperation/rw_by_line.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../../common/FileOperation/rw_by_line.c

StateMachine.o:../../../common/StateMachine/StateMachine.c   ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../../../common/StateMachine/StateMachine.c
            
ProtocolRecv.o :../ProtocolRecv.c  ../ProtocolRecv.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../ProtocolRecv.c
  
test_main.o : test_main.cc   ../ProtocolRecv.h    ../../../common/StateMachine/StateMachine.h 
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_main.cc
						
clean:
	rm $(TARGET) test_main.o ProtocolRecv.o StateMachine.o rw_by_line.o char_to_num.o loop_queues.o