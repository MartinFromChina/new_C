DLIBS = ../../../google_test/lib/libgmock.a
TARGET = battery

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

$(TARGET) : ../test/test_main.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

test_main.o : ../test/test_main.cc ../battery.h   ../test/tdd_common.h  ../../../../module/common/x_cross_platform.h
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c ../test/test_main.cc
						

	clean:
	rm $(TARGET) test_main.o