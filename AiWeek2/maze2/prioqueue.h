#ifndef PRIOQUEUE_H
#define PRIOQUEUE_H

#include "state.h"

#define MAXQ 50000

typedef struct Pqueue {
  unsigned int qsize;
  State states[MAXQ];
} Pqueue;

void initPqueue(Pqueue *pq);
int getPqueueSize(Pqueue pq);
int isEmptyPqueue(Pqueue pq);
void pushPqueue(Pqueue *pq, State s);
void topPqueue(Pqueue pq, State *s);
void popPqueue(Pqueue *pq);

// Returns 1 if the same position is found in priority queue, 0 otherwise
int existsInPqueue(Pqueue pq, State state);

#endif
