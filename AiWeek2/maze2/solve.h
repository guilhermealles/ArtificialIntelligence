#ifndef SOLVE_H
#define SOLVE_H

#include "maze.h"
#include "state.h"
#include "prioqueue.h"

void solveMaze(Maze maze);
void solveMazeDFS(Maze maze);
void solveMazeBFS(Maze maze);
void solveMazeIDA(Maze maze);
void solveMazeDLS(Maze *maze, int depth_limit, int *visit_count_ptr, int *push_count_ptr);
void solveMazeAstar(Maze maze);
int aStarEvaluate(State s, Position goal);
void solveMazeDFSNC(Maze maze);
void solveMazeBFSNC(Maze maze);
void solveMazeIDANC(Maze maze);
void solveMazeDLSNC(Maze *maze, int depth_limit, int *visit_count_ptr, int *push_count_ptr);
void solveMazeAstarNC(Maze maze);

#endif