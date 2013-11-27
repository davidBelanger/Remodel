CC=g++
CFLAGS=-I/opt/local/include/ -L/opt/local/lib/ -lboost_regex-mt -lssl -lcrypto -ltbb
DEPS = fileParsing.h


test1: test.cpp fileParsing.cpp fileStatus.cpp
	g++  -g -o test1 test.cpp fileParsing.cpp $(CFLAGS)
clean:
	rm test1
