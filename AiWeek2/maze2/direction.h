#ifndef DIRECTION_H
#define DIRECTION_H

/* Implements the enumeration type Direction. */

typedef enum Direction { /* Defines UP, DOWN, .... */
	NORTH=1, EAST=2, SOUTH=4, WEST=8, UP=16, DOWN=32
} Direction;

/* returns an ASCII string respresentation of a direction */
char *convertDirectionToString(Direction dir);

#endif
