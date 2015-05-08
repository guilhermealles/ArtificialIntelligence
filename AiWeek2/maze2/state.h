#ifndef STATE_H

#define STATE_H
/* Implements the data type Maze.
 * State is a data type that represents a possible state of a 
 * Maze. It stores current position, moved path and total cost
 * of reaching the current position.
 * 
 * An extra integer stores search priority; the higher it is, 
 * the earlier it will be searched (may be negative).
 */

#include "direction.h"
#include "position.h"
#include "path.h"

typedef struct State {
  Position pos;
  Path path;
  int cost;
  int priority;
} State;

/* initialize state s (priority=0, cost=0, path=empty) */
void initState(State *s);

/* print the state-structure s on the screen */
void printState(State s);

/* Sets pos as the the current position of the state s */
void setStatePosition(State *s, Position pos);

/* Returns the current position of the state s in pos */
void getStatePosition(State s, Position *pos);

/* Returns the path of the state s in path */
void getStatePath(State s, Path *path);

/* Returns the length of the path travelled in state s */
int getStatePathLength(State s);

/* Returns the cost of the path travelled in state s */
int getStateCost(State s);

/* Returns the priority of state s */
int getStatePriority(State s);

/* Set the priority of state s to pr */
void setStatePriority(State *s, int pr);

/* copy a state */
void copyState(State *dest, State source);

#endif

