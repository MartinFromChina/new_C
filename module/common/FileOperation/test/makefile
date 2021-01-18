DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 
TARGET = fileOP

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : test_main.o rw_by_line.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(DLIBS)

rw_by_line.o :../rw_by_line.c  ../rw_by_line.h 	
	$(CXX) $(CXXFLAGS)  -I../../../../module/common   \
            -c ../rw_by_line.c
  
test_main.o : test_main.cc   ../rw_by_line.h  
	$(CXX) $(CXXFLAGS)  -I../../../../module/common\
            -c test_main.cc
						
clean:
	rm $(TARGET) test_main.o rw_by_line.o