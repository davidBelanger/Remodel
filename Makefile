CC=g++
CFLAGS=-I/opt/local/include/ -L/opt/local/lib/ -lboost_regex-mt -lssl -lcrypto -ltbb
DEPS = fileParsing.h fileStatus.h


test1: test.cpp fileParsing.cpp fileStatus.cpp
	g++  -g -o test1 test.cpp fileParsing.cpp fileStatus.cpp $(CFLAGS)
clean:
	rm test1
