CC=g++
INCLUDE=
CFLAGS=-Wall -fPIC

all: libmadlsh.so

tmp:  
	mkdir tmp

tmp/libmadlsh.o: tmp madlsh.cpp madlsh.h
	$(CC) -c -std=c++0x madlsh.cpp -o $@ ${CFLAGS} ${INCLUDE}

libmadlsh.so: tmp tmp/libmadlsh.o 
	$(CC) -std=c++0x -shared -o $@ tmp/*.o ${INCLUDE}
#	cp *.so ../lib

clean:
	rm -rf tmp
	rm libmadlsh.so