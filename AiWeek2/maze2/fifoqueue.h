#ifndef FIFO_H
#define FIFO_H

#include "prioqueue.h"

typedef struct FifoQueue {
	int priority_counter;
	Pqueue queue;
} FifoQueue;

void initFifoQueue(FifoQueue *queue);
int getFifoQueueSize(FifoQueue queue);
int isEmptyFifoQueue(FifoQueue queue);
void pushFifoQueue(FifoQueue *queue, State s);
void topFifoQueue(FifoQueue queue, State *state);
void popFifoQueue(FifoQueue *queue);

#endif