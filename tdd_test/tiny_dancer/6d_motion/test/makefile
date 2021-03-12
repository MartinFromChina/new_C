#DLIBS = ../../../google_test/lib/libgmock.a
DLIBS = 
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = finally.so

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/finally.o	
	gcc $^ -shared -fPIC -o finally.so 
	#$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)


./obj/finally.o :../c/finally.c   ../c/finally.h 	
	gcc -fPIC -c ../c/finally.c -o $@
					
clean:
	rm 		./obj/finally.o 	finally.so				

