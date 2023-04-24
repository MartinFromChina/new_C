DLIBS = ../../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT
TARGET = generator

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread 
CXXFLAGS += $(DEFS)

$(TARGET) :  ./obj/test_main.o  ./obj/MultiTaskGenerator.o ./obj/InsertLogDebug.o ./obj/AppCommon.o  
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)
       
./obj/AppCommon.o : ../../../common/AppCommon.c ../../../common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../common/InsertLog/InsertLogDebug.c ../../../common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../../../common/InsertLog/InsertLogDebug.c -o $@

./obj/test_main.o : test_main.c 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c test_main.c -o $@

./obj/MultiTaskGenerator.o : ../MultiTaskGenerator.c 
	$(CXX) $(CXXFLAGS) -I../../../common    \
       -c ../MultiTaskGenerator.c -o $@

clean:
	rm 	 ./obj/test_main.o  ./obj/MultiTaskGenerator.o ./obj/InsertLogDebug.o  ./obj/AppCommon.o 

