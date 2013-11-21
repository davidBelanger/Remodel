CC=g++
CFLAGS=-lssl -L /usr/include/boost/ -lboost_regex
DEPS = fileParsing.h


test1: test.cpp fileParsing.cpp
	g++  -o test1 test.cpp fileParsing.cpp $(CFLAGS)
clean:
	rm test