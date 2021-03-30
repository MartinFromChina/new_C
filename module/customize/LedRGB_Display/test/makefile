DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET1 = RGB_Display.so
TARGET2 = RGB_Display

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

OBJ = ./obj/LedDisplay.o  ./obj/test_main.o
# SRC_FILES = $(filter-out ./obj/Motion_test.o, $(OBJ))


$(TARGET1) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $^ $(DLIBS)
#	$(CXX) $(CXXFLAGS) $(SRC_FILES) -shared -fPIC -o $@ 

./obj/LedDisplay.o : ../LedDisplay.c ../LedDisplay.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c ../LedDisplay.c -o $@

./obj/test_main.o : test_main.cc 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
      -fPIC -c test_main.cc -o $@
clean:
	rm 		./obj/LedDisplay.o  	 ./obj/test_main.o	
	

