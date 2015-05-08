#include <stdio.h>
#include <stdlib.h>

#include "maze.h"
#include "state.h"
#include "prioqueue.h"

void solveMaze(Maze maze) {
  Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
  Pqueue fringe;
  Position pos, goal;
  State state, successor;
  int i, priority;
  
  getPosition(maze, &pos);
  getGoal(maze, &goal);

  getMazeState(&state, maze);
  priority = random();
  setStatePriority(&state, priority);
  
  initPqueue(&fringe);
  pushPqueue(&fringe, state);
    
  while (!isEmptyPqueue(fringe) && !isSolved(maze)) {
    topPqueue(fringe, &state);
    popPqueue(&fringe);  
    setMazeState(&maze, state);    
    printState(state);
    for (i=0; i<6; i++) {
      if (canMoveTo(maze, dir[i])) {
        /* make move in direction dir[i] */
        makeMove(&maze, dir[i]);
        /* show result */
#if 0
        printf("makeMove %s\n", convertDirectionToString(dir[i])); 
        printMaze(maze);
#endif
        /* get new state (i.e. is a successor) */
        getMazeState(&successor, maze);
        /* show new state */
#if 0
        printState(successor);
#endif
        /* set priority of successor an put it in the fringe */
        priority = random();
        setStatePriority(&successor, priority);
        pushPqueue(&fringe, successor);
        /* undo move */
        setMazeState(&maze, state);
      }
    }
  }
  if (isSolved(maze)) {
   printf ("MAZE SOLVED: Path=");
   printPathWithNewline(maze.path);
  } else {
	  printf ("NO PATH FOUND\n");
  }
}

int main(int argc, char *argv[]) {
  Maze maze;
  Position pos, goal;
  
  readMazeFromFile(&maze, "maze3D.txt");

  getPosition(maze, &pos);
  printf ("The initial position is ");
  printPositionWithNewline(pos);

  getGoal(maze, &goal);
  printf ("The goal is at ");
  printPositionWithNewline(goal);

  printMaze(maze);

  solveMaze(maze);
  
  return EXIT_SUCCESS;
}
