#include <stdio.h>
#include <stdlib.h>

#include "maze.h"

int main(int argc, char *argv[]) {
	Maze maze;

	readMazeFromFile(&maze, "maze3D.txt");

	unsigned int solution_count = 13;
	Direction solution[] = {EAST, NORTH, EAST, SOUTH, EAST, NORTH, UP, WEST, WEST, NORTH, NORTH, DOWN, SOUTH};

	Position position, goal;
	getPosition(maze, &position);
	getGoal(maze, &goal);

	printf("Current position: "); printPositionWithNewline(position);
	printf("Goal: "); printPositionWithNewline(goal); printf("\n");

	printMaze(maze);

	printPossibleMoves(maze);
	printf("\n");

	for (int i=0; i < solution_count; i++) {
		if (makeMove(&maze, solution[i]) == 1) {
			if (isSolved(maze)) {
				printf("Maze solved!\n");
				printPathWithNewline(maze.path);
				break;
			}
			else {
				printf("Movement successful, maze not solved.\n");
			}
		}
		else {
			printf("Error moving from: "); printPosition(maze.pos); printf (" to %s\n", convertDirectionToString(solution[i]));
		}
	}
	return EXIT_SUCCESS;
}
