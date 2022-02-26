DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf
TARGET = wav_2_c

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/wave_to_c_test.o ./obj/InsertLogDebug.o ./obj/wav_to_c.o  
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/wave_to_c_test.o : wave_to_c_test.cc 
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c wave_to_c_test.cc -o $@
            
./obj/InsertLogDebug.o : ../../../../module/common/InsertLog/InsertLogDebug.c ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../../module/common    \
       -c ../../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/wav_to_c.o : ../wav_to_c.c ../wav_to_c.h  ../../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../wav_to_c.c -o $@
						
clean:
	rm 		./obj/wave_to_c_test.o ./obj/InsertLogDebug.o  ./obj/wav_to_c.o 