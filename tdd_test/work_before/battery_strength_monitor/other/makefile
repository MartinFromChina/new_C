DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1
TARGET = battery

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : test_main.o battery.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

battery.o :../battery.c  ../battery.h ../test/test_main.h		
	$(CXX) $(CXXFLAGS)  -I../../../../module/common  -I../../../google_test/include/gtest/include/gtest -I../../../google_test/include/include/gmock \
            -c ../battery.c
  
test_main.o : ../test/test_main.cc ../test/test_main.h  ../battery.h   ../test/tdd_common.h   
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c ../test/test_main.cc
						
clean:
	rm $(TARGET) test_main.o battery.o