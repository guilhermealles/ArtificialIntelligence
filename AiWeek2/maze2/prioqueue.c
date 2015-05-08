#include <stdio.h>
#include <stdlib.h>

#include "misc.h"
#include "prioqueue.h"

void initPqueue(Pqueue *pq) {
  pq->qsize = 0;
}

static int parent(int i) {
  return i/2;
}

static int left(int i) {
  return 2*i;
}

static int right(int i) {
  return 2*i + 1;
}

int getPqueueSize(Pqueue pq) {
  return pq.qsize;
}

int isEmptyPqueue(Pqueue pq) {
  return (pq.qsize == 0 ? 1 : 0);
}


void pushPqueue(Pqueue *pq, State s) {
  int idx, par, key = s.priority;
  State *tree = pq->states;
  pq->qsize++;
  if (pq->qsize >= MAXQ) {
    error("Error: trying to apply insertPqueue on an queue "
          "that already contains QMAX(=%d) elements.\n", MAXQ);
  }
  idx = pq->qsize;
  par = parent(idx);
  while ((idx > 1) && (tree[par].priority < key)) {
    tree[idx]= tree[par];
    idx = par;
    par = parent(par);
  };
  copyState(&tree[idx], s);
}

static void heapify(int idx, int qsize, State *tree) {
  int largest, leftChild, rightChild;
  
  leftChild = left(idx);
  rightChild = right(idx);

  if ((leftChild <= qsize) && 
      (tree[leftChild].priority > tree[idx].priority)) {
    largest = leftChild;
  } else {
    largest = idx;
  }

  if ((rightChild <= qsize) && 
      (tree[rightChild].priority > tree[largest].priority)) {
    largest = rightChild;
  }
  
  if (largest != idx) {
    State h = tree[idx];
    tree[idx] = tree[largest];
    tree[largest] = h;
    heapify(largest, qsize, tree);
  } 
}

void topPqueue(Pqueue pq, State *s) {
  if (pq.qsize < 1) {
    error("Error: trying to apply topPqueue on an empty queue.\n");
  }
  *s = pq.states[1];
}

void popPqueue(Pqueue *pq) {
  if (pq->qsize < 1) {
    error("Error: trying to apply popPqueue on an empty queue.\n");
  }
  pq->states[1] = pq->states[pq->qsize--];
  heapify(1, pq->qsize, pq->states);
}
