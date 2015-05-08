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

#endif
