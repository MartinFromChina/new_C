DLIBS = ../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1
TARGET = thread

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
#-std=c++11
CXXFLAGS += $(DEFS)

$(TARGET) : thread_test.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)
  
thread_test.o : thread_test.cc thread_test.h   
	$(CXX) $(CXXFLAGS)  \
            -c thread_test.cc
						
clean:
	rm $(TARGET) thread_test.o
