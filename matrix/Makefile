CC=gcc
CFLAGS=-I. -Wall -g
LDFLAGS=-L.
LIBS=-lmatmul
OBJECTS = main.o


all: libmatmul.so matrix.x
	
matrix.x: ${OBJECTS}
	${CC} -o matrix.x ${OBJECTS} ${LDFLAGS} ${LIBS}

matrix.o: main.c
	${CC} ${CFLAGS} -c main.c

libmatmul.so: matrix_lib.c
	${CC} -fPIC  $(CFLAGS) matrix_lib.c -o $@ ${LDFLAGS} -shared

clean:
	rm -f ${OBJECTS} matrix.x libmatmul.so
