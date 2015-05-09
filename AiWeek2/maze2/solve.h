#ifndef SOLVE_H
#define SOLVE_H

#include "maze.h"
#include "state.h"
#include "prioqueue.h"

void solveMaze(Maze maze);
void solveMazeDFS(Maze maze);
void solveMazeBFS(Maze maze);
void solveMazeIDA(Maze maze);
Maze solveMazeDLS(Maze maze, int depth_limit, int *partial_visit_count, int *partial_push_count);
void solveMazeAstar(Maze maze);

#endif