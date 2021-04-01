DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET2 = RGB_Display

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

OBJ = main.o  StateMachine.o      
# SRC_FILES = $(filter-out ./obj/Motion_test.o, $(OBJ))


$(TARGET2) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $^ $(DLIBS)

StateMachine.o : ../../../module/common/StateMachine/StateMachine.c ../../../module/common/StateMachine/StateMachine.h
	$(CXX) $(CXXFLAGS) -I../../../module/common    \
     -fPIC -c ../../../module/common/StateMachine/StateMachine.c -o $@  

main.o : main.cc
	$(CXX) $(CXXFLAGS) -I../../../module/common    \
     -fPIC -c main.cc -o $@   


clean:
	rm 		StateMachine.o  
	

