CC=gcc
CFLAGS=-Wall -g
INCLUDES=direction.h  maze.h  misc.h  position.h path.h state.h prioqueue.h solve.h

OBJECTS=main.o  maze.o  misc.o  position.o direction.o path.o state.o prioqueue.o solve.o

all: ${OBJECTS} ${INCLUDES}
	gcc -o main ${OBJECTS}

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
