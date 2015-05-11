#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"

#define RANGE 1000000

int alreadyVisited(State s, State *visited, int visited_count) {
  for (int i=0; i<visited_count-1; i++) {
    if (s.value == visited[i].value) {
      //printf("Node Already Visited!\n");
      return 1;
    }
  }
  return 0;
}

Fringe insertValidSucc(Fringe fringe, int value, State *visited, int visited_count, char *operation) {
    State s;
    s.value = value;
    s.parent = &visited[visited_count-1];
    strcpy(s.operation, operation);
    if ((value < 0) || (value > RANGE) || alreadyVisited(s, visited, visited_count)) {
        /* ignore states that are out of bounds */
        return fringe;
    }
    return insertFringe(fringe, s);
}

void printPath(State s, int visited_count) {
    State states[visited_count];
    int path_count=0;
    
    State *list = &s;
    // puts all the nodes in the path in an array
    while (list->parent != NULL) {
        states[path_count] = *list;
        path_count++;
        list = list->parent;
    }
    
    printf("\n");
    // Loops through the array backwards to get the path in the correct order
    printf("%d ", states[path_count-1].value);
    for (int i=1; i<path_count; i++) {
        printf("(%s)-> %d ", states[path_count-1-i].operation, states[path_count-1-i].value);
    }
    printf("\n");
}

void search(int mode, int start, int goal) {
    Fringe fringe;
    State state;
    int goalReached = 0;
    int value;

    // Create a closed list to keep visited states
    State *visited = (State*) malloc(sizeof(State)*2147483645); // Allocate the biggest possible integer number of states
    int visited_count = 0;

    fringe = makeFringe(mode);
    state.value = start;
    strcpy(state.operation, "\0");
    state.parent = NULL;
    
    fringe = insertFringe(fringe, state);
    while (!isEmptyFringe(fringe)) {
        /* get a state from the fringe */
        fringe = removeFringe(fringe, &state);
        visited[visited_count] = state;
        visited_count++;
        /* is state the goal? */
        value = state.value;
        if (value == goal) {
            goalReached = 1;
            break;
        }
        /* insert neighbouring states */
        fringe = insertValidSucc(fringe, value+1, visited, visited_count, "+1"); /* rule n->n + 1      */
        fringe = insertValidSucc(fringe, 2*value, visited, visited_count, "*2"); /* rule n->2*n        */
        fringe = insertValidSucc(fringe, 3*value, visited, visited_count, "*3"); /* rule n->3*n        */
        fringe = insertValidSucc(fringe, value-1, visited, visited_count, "-1"); /* rule n->n - 1      */
        fringe = insertValidSucc(fringe, value/2, visited, visited_count, "/2"); /* rule n->floor(n/2) */
        fringe = insertValidSucc(fringe, value/3, visited, visited_count, "/3"); /* rule n->floor(n/3) */
    }
    if (goalReached == 0) {
        printf("goal not reachable ");
    } else {
        printf("goal reached ");
        printPath(state, visited_count);
    }
    printf("(%d nodes visited)\n", visited_count);
    showStats(fringe);
    deallocFringe(fringe);
}

int main(int argc, char *argv[]) {
    int start, goal, fringetype;
    /*
    if ((argc == 1) || (argc > 4)) {
        fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
        return EXIT_FAILURE;
    }
    fringetype = 0;
  
    if ((strcmp(argv[1], "STACK") == 0) || (strcmp(argv[1], "LIFO") == 0)) {
        fringetype = STACK;
    } else if (strcmp(argv[1], "FIFO") == 0) {
        fringetype = FIFO;
    } else if ((strcmp(argv[1], "HEAP") == 0) || (strcmp(argv[1], "PRIO") == 0)) {
        fringetype = HEAP;
    }
    if (fringetype == 0) {
        fprintf(stderr, "Usage: %s <STACK|FIFO|HEAP> [start] [goal]\n", argv[0]);
        return EXIT_FAILURE;
    }
     */
    
    fringetype = STACK;
    start = 0;
    goal = 42;
    if (argc == 3) {
        goal = atoi(argv[2]);
    } else if (argc == 4) {
        start = atoi(argv[2]);
        goal = atoi(argv[3]);
    }

    printf("Problem: route from %d to %d\n", start, goal);
    search(fringetype, start, goal);
    return EXIT_SUCCESS;
}
