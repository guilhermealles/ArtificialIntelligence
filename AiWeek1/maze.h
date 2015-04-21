#ifndef MAZE_H
#define MAZE_H

/* Implements the data type Maze. */

#include "position.h"
#include "direction.h"
#include "path.h"

/* Maximal dimensions of a maze: MAXDIM x MAXDIM x MAXDIM */
#define MAXDIM 50

typedef struct Maze {
  Position pos;   /* location of the current position */
  Position goal;  /* location of the goal cell */
  int dim[3];     /* dimensions of the maze: width, height, depth */
  int cost;       /* cost of the path travelled so far */
  Path path;      /* path travelled from the initial position to pos */
  Direction cell[MAXDIM][MAXDIM][MAXDIM];   /* the cells of the maze  */
} Maze;

/* read a Maze from a file */
void readMazeFromFile(Maze *maze, char filename[]);

/* print maze on screen */
void printMaze(Maze maze);

/* Returns the current position in the maze in pos */
void getPosition(Maze maze, Position *pos);

/* Returns the location of the goal state in goal */
void getGoal(Maze maze, Position *goal);

/* returns true (1) if we can move in direction dir from the
 * current location, false(0) otherwise */
int canMoveTo(Maze maze, Direction dir);

/* prints all possible moves (in the current location) on the screen */
void printPossibleMoves(Maze maze);

/* returns true(1) if the goal state is reached, false otherwise */
int isSolved(Maze maze);

/* make a move in the specified direction. Returns true(1) if
 * this is indeed possible, false(0) otherwise.
 */
 int makeMove(Maze *maze, Direction dir);

/* returns the cost of the path travelled from the initial position
 * to the current location */
 int getCost(Maze maze);

#endif
