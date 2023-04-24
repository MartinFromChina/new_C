SYS_SW_WIN64=0
variable_0=0
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT -DWINDOWS_64_SYSTEM=$(SYS_SW_WIN64)      \
		-DCP_EVENT_HSM_DEEPTH=2    -DCP_RECV_HSM_DEEPTH=2   -DCP_SEND_HSM_DEEPTH=2
TARGET = test

ifeq ($(SYS_SW_WIN64),$(variable_0)))
	DLIBS = ../../../google_test/lib/libgmock.a
else
	DLIBS = ../../../google_test/lib/libgmock_win64.a
endif

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

$(TARGET) :  ./obj/cp_drive.o ./obj/x_task_common.o ./obj/InsertLogDebug.o ./obj/AppCommon.o  ./obj/tdd_test.o  \
				./obj/h_state_machine.o    ./obj/x_task_common.o   ./obj/loop_queues.o    ./obj/priority_queues.o  \
				./obj/bit_operation.o        ./obj/tdd_event_test.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/x_task_common.o : ../../../component/const_loop_scheduler/x_task_common.c  ../../../component/const_loop_scheduler/x_task_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c ../../../component/const_loop_scheduler/x_task_common.c -o $@

./obj/h_state_machine.o : ../../../component/const_loop_scheduler/HierarchicalSM/h_state_machine.c  ../../../component/const_loop_scheduler/HierarchicalSM/h_state_machine.h 
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c ../../../component/const_loop_scheduler/HierarchicalSM/h_state_machine.c -o $@

./obj/cp_drive.o : ../cp_drive.c tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../cp_drive.c -o $@

./obj/x_task_common.o : ../../../component/const_loop_scheduler/x_task_common.c  ../../../component/const_loop_scheduler/x_task_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common \
            -c ../../../component/const_loop_scheduler/x_task_common.c -o $@
       
./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/loop_queues.o : ../../../common/loopqueue/loop_queues.c ../../../common/loopqueue/loop_queues.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/loopqueue/loop_queues.c -o $@

./obj/priority_queues.o : ../../../common/priorityqueue/priority_queues.c ../../../common/priorityqueue/priority_queues.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/priorityqueue/priority_queues.c -o $@

./obj/bit_operation.o : ../../../common/math/bit_operation.c ../../../common/math/bit_operation.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/math/bit_operation.c -o $@

./obj/tdd_test.o : tdd_test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c tdd_test.cc -o $@

./obj/tdd_event_test.o : tdd_event_test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c tdd_event_test.cc -o $@	   

clean:
	rm 		./obj/cp_drive.o ./obj/x_task_common.o ./obj/InsertLogDebug.o ./obj/AppCommon.o  ./obj/tdd_test.o   \
			./obj/h_state_machine.o    ./obj/x_task_common.o   ./obj/loop_queues.o    ./obj/priority_queues.o   \
			./obj/bit_operation.o       ./obj/tdd_event_test.o

