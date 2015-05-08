#ifndef POSITION_H
#define POSITION_H

#include <stdio.h>

/* Implements the data type Position, in which 3D-coordinates
 * are represented.
 */
 
typedef struct Position {
  int x, y, z;
} Position;


/* Read a position from file f. The result is placed in p.
 * The function returns 1 on succes, 0 otherwise.
 */
void readPositionFromFile(FILE *f, Position *p);

/* Assign (x,y,z)  to p */
void setPosition(Position *p, int x, int y, int z);

/* print the coordinate p on the screen */
void printPosition(Position p);

/* like printPosition, followed by a newline */
void printPositionWithNewline(Position p);

/* returns true(1) if p==q, otherwise false(0) */
int isEqualPosition(Position p, Position q);

/* return true(1) if p==(x,y,z),  otherwise false(0) */
int isEqualPosition2(Position p, int x, int y, int z);

/* implements assignment p=q */
void copyPosition(Position *p, Position q);

#endif
