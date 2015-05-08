#include "direction.h"
#include "position.h"
#include "path.h"
#include "state.h"

/* initialize state s (priority=0, cost=0, path=empty) */
void initState(State *s) {
  s->priority = 0;
  s->cost = 0;
  initPath(&s->path);
}


/* print the state-structure s on the screen */
void printState(State s) {
  printf("State=[\n");
  printf("  pos="); printPosition(s.pos); printf("\n");
  printf("  cost=%d\n", s.cost);
  printf("  priority=%d\n", s.priority);
  printf("  path="); printPathWithNewline(s.path);
  printf("]\n");
}

/* Returns the current position of the state s in pos */
void getStatePosition(State s, Position *pos) {
  copyPosition(pos, s.pos);
}

/* Sets pos as the the current position of the state s */
void setStatePosition(State *s, Position pos) {
  copyPosition(&s->pos, pos);
}

/* Returns the path of the state s in path */
void getStatePath(State s, Path *path) {
  *path = s.path;
}

/* Returns the length of the path travelled in state s */
int getStatePathLength(State s) {
  return s.path.length;
}

/* Returns the cost of the path travelled */
int getStateCost(State s) {
  return s.cost;
}

/* Returns the priority of state s */
int getStatePriority(State s) {
  return s.priority;
}

/* Set the priority of state s to pr */
void setStatePriority(State *s, int pr) {
  s->priority = pr;
}

/* copy a state */
void copyState(State *dest, State source) {
  dest->pos = source.pos;
  copyPath(&dest->path, source.path);
	dest->cost = source.cost;
	dest->priority = source.priority;
}



