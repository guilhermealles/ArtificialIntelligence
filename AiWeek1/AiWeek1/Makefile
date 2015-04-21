CC=gcc
CFLAGS=-Wall
INCLUDES=direction.h  maze.h  misc.h  position.h path.h

OBJECTS=main.o  maze.o  misc.o  position.o direction.o path.o

all: ${OBJECTS} ${INCLUDES}
	gcc -o main ${OBJECTS}

clean:
	rm -f *~
	rm -f #*#
	rm -f *.o
