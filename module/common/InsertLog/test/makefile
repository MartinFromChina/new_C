DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = Log

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : test_main.o InsertLogDebug.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

InsertLogDebug.o :../InsertLogDebug.c  ../InsertLogDebug.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../InsertLogDebug.c
  
test_main.o : test_main.cc   ../InsertLogDebug.h  
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_main.cc
						
clean:
	rm $(TARGET) test_main.o InsertLogDebug.o