#DLIBS = ../../../google_test/lib/libgmock.a
DLIBS = 
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DUSE_INSERT_DEBUG=1
TARGET = finally.so

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/finally.o	 ./obj/InsertLogDebug.o ./obj/DataMonitor.o ./obj/MotionFusion.o
	$(CXX) $(CXXFLAGS) $^ -shared -fPIC -o $@ $(DLIBS)

./obj/finally.o :../c/finally.c   ../c/finally.h 	../c/DataMonitor.h 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
	-fPIC -c ../c/finally.c -o $@

./obj/DataMonitor.o :../c/DataMonitor.c   ../c/DataMonitor.h 	
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
	-fPIC -c ../c/DataMonitor.c -o $@

./obj/DataConvert.o :../c/DataConvert.c   ../c/DataConvert.h 	
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
	-fPIC -c ../c/DataConvert.c -o $@

./obj/MotionFusion.o :../c/MotionFusion.c   ../c/MotionFusion.h 	
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
	-fPIC -c ../c/MotionFusion.c -o $@

./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../../module/common    \
      -fPIC -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@					
clean:
	rm 		./obj/finally.o  ./obj/DataMonitor.o finally.so	./obj/MotionFusion.o			

