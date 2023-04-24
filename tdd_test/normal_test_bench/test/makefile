DLIBS = ../../google_test/lib/libgmock.a
DEFS = -DUSE_TDD_MOCK=1 -DLOG_DEBUG_METHOD=printf -DARM_COMPILER_PRESENT=1
TARGET = test

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread
CXXFLAGS += $(DEFS)

$(TARGET) : ./obj/test_main.o ./obj/InsertLogDebug.o ./obj/AppCommon.o ./obj/bit_operation.o ./obj/c_test_bench.o   ./obj/cpp_test_bench.o 
	$(CXX) $(CXXFLAGS) -o $@  $^ $(DLIBS)

./obj/test_main.o : test_main.cc 
	$(CXX) $(CXXFLAGS) -I../../../module/common    \
       -c test_main.cc -o $@


./obj/bit_operation.o : ../../../module/common/Math/bit_operation.c ../../../module/common/Math/bit_operation.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../../../module/common/Math/bit_operation.c -o $@

./obj/AppCommon.o : ../../../module/common/AppCommon.c ../../../module/common/AppCommon.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../../../module/common/AppCommon.c -o $@

./obj/InsertLogDebug.o : ../../../module/common/InsertLog/InsertLogDebug.c ../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../../module/common    \
       -c ../../../module/common/InsertLog/InsertLogDebug.c -o $@

./obj/c_test_bench.o : ../c_test_bench.c ../test_bench.h  ../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../module/common    \
       -c ../c_test_bench.c -o $@

./obj/cpp_test_bench.o : ../cpp_test_bench.cc ../test_bench.h  ../../../module/common/InsertLog/InsertLogDebug.h
	$(CXX) $(CXXFLAGS) -I../../../module/common    \
       -c ../cpp_test_bench.cc -o $@
						
clean:
	rm 		./obj/test_main.o ./obj/InsertLogDebug.o  ./obj/AppCommon.o ./obj/bit_operation.o ./obj/c_test_bench.o  ./obj/cpp_test_bench.o 