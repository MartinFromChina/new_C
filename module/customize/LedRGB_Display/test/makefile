DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1  -DUSE_LOG_DEBUG_IN_GOOGLE_TEST=1
TARGET1 = RGB_Display.so
TARGET2 = RGB_Display

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

OBJ = ./obj/test_main.o  ./obj/LedDisplay.o  ./obj/loop_queues.o   ./obj/StateMachine.o   ./obj/InsertLogDebug.o     
# SRC_FILES = $(filter-out ./obj/Motion_test.o, $(OBJ))


$(TARGET1) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $^ $(DLIBS)
#	$(CXX) $(CXXFLAGS) $(SRC_FILES) -shared -fPIC -o $@ 


./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/loop_queues.o : ../../../../module/common/loopqueue/loop_queues.c ../../../../module/common/loopqueue/loop_queues.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c ../../../../module/common/loopqueue/loop_queues.c -o $@

./obj/StateMachine.o : ../../../../module/common/StateMachine/StateMachine.c ../../../../module/common/StateMachine/StateMachine.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
     -fPIC -c ../../../../module/common/StateMachine/StateMachine.c -o $@  
            
./obj/LedDisplay.o : ../LedDisplay.c ../LedDisplay.h  ../../../../module/common/StateMachine/StateMachine.h ../../../../module/common/AppError.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c ../LedDisplay.c -o $@

./obj/test_main.o : test_main.cc 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c test_main.cc -o $@
clean:
	rm 		./obj/LedDisplay.o  	 ./obj/test_main.o	./obj/loop_queues.o  ./obj/StateMachine.o  ./obj/InsertLogDebug.o
	

