DLIBS = ../../../google_test/lib/libgmock.a
CXXFLAGS += -g -Wall -Wextra -pthread

test:StateMachine.o test_main.o
	g++ $(CXXFLAGS) -o test StateMachine.o test_main.o $(DLIBS)

StateMachine.o : ../StateMachine.c ../StateMachine.h ../../x_cross_platform.h ../../CommonMarco.h ../../AppError.h 
	g++ $(CXXFLAGS) -I../../../common -c ../StateMachine.c

test_main.o : test_main.cc 
	g++ $(CXXFLAGS)  -c test_main.cc

clean :
	rm 	test StateMachine.o test_main.o