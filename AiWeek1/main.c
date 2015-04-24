#include <stdio.h>
#include <stdlib.h>

#include "maze.h"

int main(int argc, char *argv[]) {
	Maze maze;

	readMazeFromFile(&maze, "maze3D.txt");

	unsigned int solution_count = 13;
	Direction solution[] = {EAST, NORTH, EAST, SOUTH, EAST, NORTH, UP, WEST, WEST, NORTH, NORTH, DOWN, SOUTH};

	printMaze(maze);
	
	for (int i = 0; i < solution_count; i++) {
		if (makeMove(&maze, solution[i]) == 1) {
			printf("Successfuly moved to direction: %s\n", convertDirectionToString(solution[i]));
		}
		else {
			printf("Error moving from: "); printPosition(maze.pos); printf (" to %s\n", convertDirectionToString(solution[i]));
		}
	}

	if (isSolved(maze)) {
		printf("Success!\n");
	}
	else {
		printf("Failure :(.\n");
	}

	return EXIT_SUCCESS;
}
