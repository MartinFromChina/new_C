#DLIBS = ../../../google_test/lib/libgmock.a
DLIBS = 
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET = finally.so

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/finally.o	 ./obj/InsertLogDebug.o
	$(CXX) $(CXXFLAGS) $^ -shared -fPIC -o $@ $(DLIBS)

./obj/finally.o :../c/finally.c   ../c/finally.h 	
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
	-fPIC -c ../c/finally.c -o $@

./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../../module/common    \
      -fPIC -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@					
clean:
	rm 		./obj/finally.o 	finally.so				

