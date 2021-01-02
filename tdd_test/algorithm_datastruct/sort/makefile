DLIBS = ../../google_test/lib/libgmock.a

sort : sort_test_main.cc sort_myself_test.cc sort_myself.c 
	g++ -o sort sort_test_main.cc sort_myself_test.cc sort_myself.c -pthread $(DLIBS)
clean:
	rm sort
