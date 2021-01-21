DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = Recv

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : test_main.o ProtocolRecv.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

ProtocolRecv.o :../ProtocolRecv.c  ../ProtocolRecv.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../ProtocolRecv.c
  
test_main.o : test_main.cc   ../ProtocolRecv.h  
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_main.cc
						
clean:
	rm $(TARGET) test_main.o ProtocolRecv.o