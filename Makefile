CC=clang++
CFLAGS=-std=c++11 -stdlib=libc++  -I/opt/local/include/ -L/opt/local/lib/  -lssl -lcrypto -ltbb 
DEPS = fileParsing.h fileStatus.h

#%.o: %.c $(DEPS)
#	$(CC) -c -o $@ $< $(CFLAGS)

test1: test.cpp fileParsing.cpp fileStatus.cpp parallelBuild.cpp
	$(CC)  -g -o test1 test.cpp fileParsing.cpp fileStatus.cpp parallelBuild.cpp $(CFLAGS)

#test1: test.o fileParsing.o fileStatus.o
#	$(CC)  -g -o test1 test.o fileParsing.o fileStatus.o $(CFLAGS)
clean:
	rm test1 rm *.o

