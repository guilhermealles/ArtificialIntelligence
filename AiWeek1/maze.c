#include <stdlib.h>
#include <string.h>
#include "maze.h"
#include "misc.h"
#include "position.h"

static void initMaze(Maze *maze, int width, int height, int depth) {
	maze->dim[0] = width;
	maze->dim[1] = height;
	maze->dim[2] = depth;
	maze->cost = 0;
	initPath(&maze->path);
	memset(maze->cell, 0, MAXDIM*MAXDIM*MAXDIM*sizeof(int));
}

/* Returns the current position in the maze in pos */
void getPosition(Maze maze, Position *pos) {
	copyPosition(pos, maze.pos);
}

void printCurrentPosition (Maze maze)
{
	printf("Current location: (%d, %d, %d) \n",  maze.pos.x, maze.pos.y, maze.pos.z);
}

void setGoal(Position g, Maze *maze) {
	maze->goal = g;
}

void printGoal (Maze maze)
{
	printf("Goal position: (%d, %d, %d) \n", maze.goal.x, maze.goal.y, maze.goal.z);
}

/* Returns the location of the goal state in goal */
void getGoal(Maze maze, Position *goal) {
	copyPosition(goal, maze.goal);
}

/* returns the cost of the path travelled from the initial position
 * to the current location */
int getCost(Maze maze) {
	return maze.cost;
}

void setMazeAccess(Maze *maze, Position pos, Direction dir) {
	int x, y, z;
	x = pos.x;
	y = pos.y;
	z = pos.z;
	maze->cell[x][y][z] |= dir;
	
/* set symmetric access (what goes up, must go down..) */
	switch (dir) {
		case NORTH: maze->cell[x][y+1][z] |= SOUTH; break;
		case SOUTH: maze->cell[x][y-1][z] |= NORTH; break;
		case WEST : maze->cell[x-1][y][z] |=  EAST; break;
		case EAST : maze->cell[x+1][y][z] |=  WEST; break;
		case UP   : maze->cell[x][y][z+1] |=  DOWN; break;
		case DOWN : maze->cell[x][y][z-1] |=  UP; break;
	}
}

/* read a Maze from a file */
void readMazeFromFile(Maze *maze, char filename[]) {
	FILE *f;
	int x, y, z, c;

	f = fopen(filename, "r");
	if (f == NULL) error("Error: could not open file %s", filename);

  /* read dimensions of maze */
	if (fscanf (f, "%d %d %d\n", &x, &y, &z) != 3) syntaxError(filename);
	initMaze(maze, x, y, z);

  /* read initial position */
	readPositionFromFile(f, &maze->pos);

  /* add initial position to path */
	appendPositionToPath(&maze->path, maze->pos);

  /* read goal position */
	readPositionFromFile(f, &maze->goal);
  /* read actual cells of the maze */
	c = ' ';
  while (c != EOF) {  /* while end of file not reached ... */
	Position pos;
	readPositionFromFile(f, &pos);
	c = skipSpaces(f);
	if (c != ':') syntaxError(filename);
	do {
		c = skipSpaces(f);
		switch (c) {
        case 'E': /* EAST */
			setMazeAccess(maze, pos, EAST);
			break;
        case 'W': /* WEST */
			setMazeAccess(maze, pos, WEST);
			break;
        case 'N': /* NORTH */
			setMazeAccess(maze, pos, NORTH);
			break;
        case 'S': /* SOUTH */
			setMazeAccess(maze, pos, SOUTH);
			break;
        case 'U': /* UP */
			setMazeAccess(maze, pos, UP);
			break;
        case 'D': /* DOWN */
			setMazeAccess(maze, pos, DOWN);
			break;
			default:
			if ((c != ' ') && (c != '\t') && (c!='\n') && (c != EOF)) {
				syntaxError(filename);
			}
		}
	} while ((c != EOF) && (c!= '\n'));
	if (c == '\n') c = fgetc(f);
}
}

/* print maze on screen */
void printMaze(Maze maze) {
	int x, y, z;
	for (z=maze.dim[2]-1; z>=0; z--) {
		printf("Floor #%d\n", z);
		for (y=maze.dim[1]-1; y>=0; y--) {
			for (x=0; x<maze.dim[0]; x++) {
				if (maze.cell[x][y][z] & NORTH) {
					printf("|--| |--");
				} else {
					printf("|-------");
				}
			}
			printf("|\n");
			for (x=0; x<maze.dim[0]; x++) {
				if (maze.cell[x][y][z] & WEST) {
					printf("-       ");
				} else {
					printf("|       ");
				}
			}
			printf("|\n");
			for (x=0; x<maze.dim[0]; x++) {
				char up = (maze.cell[x][y][z] & UP ? 'U' : ' ');
				char down = (maze.cell[x][y][z] & DOWN ? 'D' : ' ');
				char mid = ' ';
				if (isEqualPosition2(maze.goal, x, y, z)) {
					mid = 'G';
				} else {
					if (isEqualPosition2(maze.pos, x, y, z)) {
						mid = 'X';
					}
				}
				if (maze.cell[x][y][z] & WEST) {
					printf("  %c %c %c ", up, mid, down);
				} else {
					printf("| %c %c %c ", up, mid, down);
				}
			}
			printf("|\n");
			for (x=0; x<maze.dim[0]; x++) {
				if (maze.cell[x][y][z] & WEST) {
					printf("-       ");
				} else {
					printf("|       ");
				}
			}
			printf("|\n");
		}
		for (x=0; x<maze.dim[0]; x++) printf("|-------");
			printf("|\n");
		printf("\n");
	}
}

/* returns true (1) if we can move in Direction dir from the
 * current location, false(0) otherwise */
int canMoveTo(Maze maze, Direction dir) {
	return (maze.cell[maze.pos.x][maze.pos.y][maze.pos.z] & dir ? 1 :0);
}

/* prints all possible moves (in the current location) on the screen */
void printPossibleMoves(Maze maze) {
	const Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};

	printCurrentPosition(maze);

	for (int i = 0; i < 6; i++)
	{
		if (canMoveTo(maze, dir[i]) == 1) // movement is possible
		{
			printf("\t%s\tpossible.\n", convertDirectionToString(dir[i]));
		}
		else
		{
			printf("\t%s\tNOT possible.\n", convertDirectionToString(dir[i]));
		}
	}  
}

/* returns true(1) if the goal state is reached, false otherwise */
int isSolved(Maze maze) {

	if (isEqualPosition(maze.pos, maze.goal)) {
		return 1;
	}
	else {
		return 0;
	}
}

/* make a move in the specified direction. Returns true(1) if
 * this is indeed possible, false(0) otherwise.
 */
 int makeMove(Maze *maze, Direction dir) {

 	if (canMoveTo(*maze, dir) == 0) {
 		return 0;
 	}
 	else {
 		// Add current position to path
 		appendPositionToPath(&(maze->path), maze->pos);

 		Position new_position = maze->pos;
 		// Set new position according to direction
 		switch (dir) {
 			case UP:
 				new_position.z += 1;
 				break;
 			case DOWN:
 				new_position.z -= 1;
 				break;
 			case NORTH:
 				new_position.y += 1;
 				break;
 			case EAST:
 				new_position.x += 1;
 				break;
 			case SOUTH:
 				new_position.y -= 1;
 				break;
 			case WEST:
 				new_position.x -= 1;
 				break;
 			default:
 				return 0;
 		}
 		// Replace old position with new position
 		copyPosition(&(maze->pos), new_position);

 		// Increment cost
 		maze->cost += 1;
 		return 1;
 	}
}
