#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "fringe.h"
#include "path.h"

#define RANGE 1000000

// Returns 1 if the list contains the value, 0 otherwise
int containsValue(State list[], int value, int list_size)
{
  for (int i = 0; i < list_size; i++)
  {
    if (list[i].value ==  value)
    {
      return 1;
    }
  }
  return 0;
}

Fringe insertValidSucc (Fringe fringe, int value, State* visited_states, int visited_states_count, char* operation, Path* path) {
  State s;

  if ((value < 0) || (value > RANGE) || containsValue(visited_states, value, visited_states_count)) {
    /* ignore states that are out of bounds and that have already been visited */
    return fringe;
  }
  s.value = value;
  s.operation = operation;
  path->path[path->length] = visited_states[visited_states_count-1];
  path->length++;

  return insertFringe(fringe, s);
}

void printPath (Path path)
{
  printf("%d ", path.path[0].value);

  for(int i = 1; i < path.length; i++)
  {
    printf(" (%s) -> ", path.path[i].operation);
    printf("%d ", path.path[i].value);
  }
}

void search(int mode, int start, int goal) {
  printf("Dentro de search\n");
  Fringe fringe;
  State state;
  int goalReached = 0;
  int visited = 0;
  int value;
  State visited_states[MAXF];
  Path path_to_goal;

  fringe = makeFringe(mode);
  state.value = start;
  state.operation = NULL;
  path_to_goal.length = 0;
  fringe = insertFringe(fringe, state);
  
  while (!isEmptyFringe(fringe)) {
    /* get a state from the fringe */
    fringe = removeFringe(fringe, &state);
    visited_states[visited] = state;
    visited++;
    /* is state the goal? */

    value = state.value;
    if (value == goal) {
      goalReached = 1;
      break;
    }

    /* insert neighbouring states */
    fringe = insertValidSucc(fringe, value+1, visited_states, visited, "+1", &path_to_goal); /* rule n->n + 1      */
    fringe = insertValidSucc(fringe, 2*value, visited_states, visited, "*2", &path_to_goal); /* rule n->2*n        */
    fringe = insertValidSucc(fringe, 3*value, visited_states, visited, "*3", &path_to_goal); /* rule n->3*n        */
    fringe = insertValidSucc(fringe, value-1, visited_states, visited, "-1", &path_to_goal); /* rule n->n - 1      */
    fringe = insertValidSucc(fringe, value/2, visited_states, visited, "/2", &path_to_goal); /* rule n->floor(n/2) */
    fringe = insertValidSucc(fringe, value/3, visited_states, visited, "/3", &path_to_goal); /* rule n->floor(n/3) */
  }
  if (goalReached == 0) {
    printf("goal not reachable ");
  } else {
    printf("goal reached ");
    printPath(path_to_goal);
  }
  printf("(%d nodes visited)\n", visited);
  showStats(fringe);
  deallocFringe(fringe);  
}

int main(int argc, char *argv[]) {
  int start, goal, fringetype;
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
