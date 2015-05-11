#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "fringe.h"

Fringe makeFringe(int mode) {
    /* Returns an empty fringe.
     * The mode can be LIFO(=STACK), FIFO, or PRIO(=HEAP)
     */
    Fringe f;
    if ((mode != LIFO) && (mode != STACK) && (mode != FIFO) &&
        (mode != PRIO) && (mode != HEAP)) {
        fprintf(stderr, "makeFringe(mode=%d): incorrect mode. ", mode);
        fprintf(stderr, "(mode <- [LIFO,STACK,FIFO,PRIO,HEAP])\n");
        exit(EXIT_FAILURE);
    }
    f.mode = mode;
    f.size = f.front = f.rear = 0; /* front+rear only used in FIFO mode */
    f.states = malloc(MAXF*sizeof(State));
    if (mode == PRIO || mode == HEAP) {
        f.priorities = malloc(MAXF*sizeof(int));
    }
    if (f.states == NULL) {
        fprintf(stderr, "makeFringe(): memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    f.maxSize = f.insertCnt = f.deleteCnt = 0;
    return f;
}

void deallocFringe(Fringe fringe) {
    /* Frees the memory allocated for the fringe */
    free(fringe.states);
    if (fringe.mode == PRIO || fringe.mode == HEAP) {
        free(fringe.priorities);
    }
}

int getFringeSize(Fringe fringe) {
  /* Returns the number of elements in the fringe 
   */
  return fringe.size;
}

int isEmptyFringe(Fringe fringe) {
  /* Returns 1 if the fringe is empty, otherwise 0 */
  return (fringe.size == 0 ? 1 : 0);
}

Fringe insertFringe(Fringe fringe, State s, ...) {
    /* Inserts s in the fringe, and returns the new fringe.
    * This function needs a third parameter in PRIO(HEAP) mode.
     */
    int priority;
    va_list argument;

    if (fringe.size == MAXF) {
        fprintf(stderr, "insertFringe(..): fatal error, out of memory.\n");
        exit(EXIT_FAILURE);
    }
    fringe.insertCnt++;
    switch (fringe.mode) {
        case LIFO: /* LIFO == STACK */
        case STACK:
            fringe.states[fringe.size] = s;
            break;
        case FIFO:
            fringe.states[fringe.rear++] = s;
            fringe.rear %= MAXF;
            break;
        case PRIO: /* PRIO == HEAP */
        case HEAP:
            /* Get the priority from the 3rd argument of this function.
             * You are not supposed to understand the following 5 code lines.
             */
            va_start(argument, s);
            priority = va_arg(argument, int);
            va_end(argument);
            fringe.states[fringe.size] = s;
            fringe.priorities[fringe.size] = priority;
            break;
    }
    fringe.size++;
    if (fringe.size > fringe.maxSize) {
        fringe.maxSize = fringe.size;
    }
    return fringe;
}

int getHighestPriorityIndex(Fringe fringe) {
    int max_index=0, max_priority=0, i=0;
    for (i=0; i<fringe.size; i++) {
        if (fringe.priorities[i] > max_priority) {
            max_priority = fringe.priorities[i];
            max_index = i;
        }
    }
    return max_index;
}

Fringe defragFringe(Fringe fringe, int index) {
    if (index < fringe.size) {
        for (int i=index+1; i<fringe.size+1; i++) {
            fringe.states[i-1] = fringe.states[i];
            fringe.priorities[i-1] = fringe.priorities[i];
        }
    }
    return fringe;
}

Fringe removeFringe(Fringe fringe, State *s) {
    /* Removes an element from the fringe, and returns it in s. 
     * Moreover, the new fringe is returned.
     */
    if (fringe.size < 1) {
        fprintf(stderr, "removeFringe(..): fatal error, empty fringe.\n");
        exit(EXIT_FAILURE);
    }
    fringe.deleteCnt++;
    fringe.size--;
    int highest_priority_index;
    switch (fringe.mode) {
        case LIFO: /* LIFO == STACK */
        case STACK:
            *s = fringe.states[fringe.size];
            break;
        case FIFO:
            *s = fringe.states[fringe.front++];
            fringe.front %= MAXF;
            break;
        case PRIO: /* PRIO == HEAP */
        case HEAP:
            highest_priority_index = getHighestPriorityIndex(fringe);
            *s = fringe.states[highest_priority_index];
            fringe = defragFringe(fringe, highest_priority_index);
            break;
    }
    return fringe;
}

void showStats(Fringe fringe) {
  /* Shows fringe statistics */
  printf("#### fringe statistics:\n");
  printf(" #size        : %7d\n", fringe.size);
  printf(" #maximum size: %7d\n", fringe.maxSize);
  printf(" #insertions  : %7d\n", fringe.insertCnt);
  printf(" #deletions   : %7d\n", fringe.deleteCnt);
  printf("####\n"); 
}
