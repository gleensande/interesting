CC=c++
CFLAGS=-c

all: main.out

main.out: matrix.o main.cpp
	$(CC) -o main.out matrix.o main.cpp

matrix.o: matrix.hpp matrix.cpp
	$(CC) matrix.hpp matrix.cpp $(CFLAGS)

clean:
	rm matrix.hpp.gch matrix.o