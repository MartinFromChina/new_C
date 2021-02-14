DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = handwriting

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/handwriting_test.o ./obj/InsertLogDebug.o ./obj/z_math.o ./obj/rw_by_line.o 
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/handwriting_test.o : handwriting_test.cc 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c handwriting_test.cc -o $@
            
./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../../module/common    \
       -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/rw_by_line.o : ../../../../module/common/FileOperation/rw_by_line.c ../../../../module/common/FileOperation/rw_by_line.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../../../../module/common/FileOperation/rw_by_line.c -o $@

./obj/z_math.o : ../z_math.c ../z_math.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../z_math.c -o $@
						
clean:
	rm 		./obj/handwriting_test.o ./obj/InsertLogDebug.o  ./obj/rw_by_line.o 