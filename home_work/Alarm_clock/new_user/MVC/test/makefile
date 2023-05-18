SYS_SW_WIN64=0
variable_0=0
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT -DWINDOWS_64_SYSTEM=$(SYS_SW_WIN64)
TARGET = test

ifeq ($(SYS_SW_WIN64),$(variable_0))
	DLIBS = ../../../../../../module/google_test/lib/libgmock.a
else
	DLIBS = ../../../../../../module/google_test/lib/libgmock_win64.a
endif

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/test.o  
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

#./obj/const_loop_scheduler.o : ../const_loop_scheduler.c  ../const_loop_scheduler.h 
#	$(CXX) $(CXXFLAGS) -I../../../common \
#            -c ../const_loop_scheduler.c -o $@

#./obj/x_task_common.o : ../x_task_common.c  ../x_task_common.h 
#	$(CXX) $(CXXFLAGS) -I../../../common \
#            -c ../x_task_common.c -o $@

#./obj/template_task.o : ../TemplateTask/template_task.c  ../TemplateTask/template_task.h 
#	$(CXX) $(CXXFLAGS) -I../../../common \
#            -c ../TemplateTask/template_task.c -o $@

#./obj/h_state_machine.o : ../HierarchicalSM/h_state_machine.c  ../HierarchicalSM/h_state_machine.h 
#	$(CXX) $(CXXFLAGS) -I../../../common \
#            -c ../HierarchicalSM/h_state_machine.c -o $@
       
#./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
#	$(CXX) $(CXXFLAGS) -I../../../common    \
#       -c ../../../common/AppCommon.c -o $@

#./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
#	$(CXX) $(CXXFLAGS) -I../../../common    \
#       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/test.o : test.cc tdd_common.h 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c test.cc -o $@

	   

clean:
	rm 		./obj/test.o 

