CC=g++
CFLAGS=-I/opt/local/include/ -L/opt/local/lib/ -lboost_regex-mt -lssl -lcrypto -ltbb
DEPS = fileParsing.h fileStatus.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test1: test.o fileParsing.o fileStatus.o
	g++  -g -o test1 test.o fileParsing.o fileStatus.o $(CFLAGS)
clean:
	rm test1
