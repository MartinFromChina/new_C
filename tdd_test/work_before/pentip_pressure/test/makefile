DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = pentip

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/pressure_test.o ./obj/InsertLogDebug.o ./obj/pentip_pressure.o ./obj/rw_by_line.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/pressure_test.o : pressure_test.cc 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c pressure_test.cc -o $@
            
./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../../module/common    \
       -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/rw_by_line.o : ../../../../module/common/FileOperation/rw_by_line.c ../../../../module/common/FileOperation/rw_by_line.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../../../../module/common/FileOperation/rw_by_line.c -o $@

./obj/pentip_pressure.o : ../pentip_pressure.c ../pentip_pressure.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../pentip_pressure.c -o $@
						
clean:
	rm 		./obj/pressure_test.o ./obj/InsertLogDebug.o ./obj/pentip_pressure.o ./obj/rw_by_line.o