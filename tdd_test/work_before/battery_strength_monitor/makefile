DLIBS = ../../../google_test/lib/libgmock.a

TARGET = battery

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

#$(TARGET) : battery.o
#	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)
	
battery.o :battery.c  battery.h ./test/test_main.h		
	$(CXX) $(CXXFLAGS)  -I../../../module/common  -I../../google_test/include/gtest/include/gtest -I../../google_test/include/include/gmock \
            -c battery.c