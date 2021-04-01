DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET2 = RGB_Display

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

OBJ =  test_main.o  StateMachine.o  
# SRC_FILES = $(filter-out ./obj/Motion_test.o, $(OBJ))


$(TARGET2) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $^ $(DLIBS)

#test_11.o : ../../../../module/common/StateMachine/test_11.c ../../../../module/common/StateMachine/test_11.h
#	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
#     -fPIC -c ../../../../module/common/StateMachine/test_11.c -o $@ 

StateMachine.o : ../../../../module/common/StateMachine/StateMachine.c ../../../../module/common/StateMachine/StateMachine.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
     -fPIC -c ../../../../module/common/StateMachine/StateMachine.c -o $@  

test_main.o : test_main.cc ../LedDisplay.h  ../../../../module/common/StateMachine/StateMachine.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common   -I../../../../module/common/StateMachine \
     -fPIC -c test_main.cc -o $@ 


clean:
	rm 		StateMachine.o  test_main.o RGB_Display  
	

