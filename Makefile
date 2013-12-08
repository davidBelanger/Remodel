CC=gcc
CFLAGS=-std=c++11 -stdlib=libc++  -I/opt/local/include/ -L/opt/local/lib/  -lssl -lcrypto -ltbb 
DEPS = fileParsing.h fileStatus.h

test1: remodel.cpp fileParsing.cpp fileStatus.cpp parallelBuild.cpp
	$(CC)  -Wall -g -o BuildRemodel remodel.cpp fileParsing.cpp fileStatus.cpp parallelBuild.cpp $(CFLAGS)

clean:
	rm BuildRemodel


