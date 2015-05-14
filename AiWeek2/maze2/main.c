#include <stdio.h>
#include <stdlib.h>

#include "maze.h"
#include "state.h"
#include "prioqueue.h"
#include "solve.h"

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

<<<<<<< Updated upstream
  solveMazeAstar(maze);
=======
  solveMazeIDA(maze);
>>>>>>> Stashed changes
  
  return EXIT_SUCCESS;
}
