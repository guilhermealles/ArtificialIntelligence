#include <stdio.h>
#include <stdlib.h>

#include "maze.h"

int main(int argc, char *argv[]) {
	Maze maze;

	readMazeFromFile(&maze, "maze3D.txt");

	printCurrentPosition(maze);
	printGoal(maze);
	printMaze(maze);
	printPossibleMoves(maze);

	return EXIT_SUCCESS;
}
