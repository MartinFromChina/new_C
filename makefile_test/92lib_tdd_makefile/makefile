# 库文件和库查找路径
DLIBS = ./google_test/lib/libgmock.a 
# 指定运行时的库文件路径
#RPATH = -Wl,-rpath=./google_test/lib

#the param -pthread is important ,the reason is below:
# Flags passed to the C++ compiler: CXXFLAGS += -g -Wall -Wextra -pthread is used when lib/libgmock is generated
test : tdd_main.cc	
	g++ -o test tdd_main.cc -pthread $(DLIBS)

clean :
	rm test 
