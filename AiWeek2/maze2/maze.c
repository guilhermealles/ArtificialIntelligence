#include <stdlib.h>
#include <string.h>
#include "maze.h"
#include "state.h"
#include "misc.h"

void initMaze(Maze *maze, int width, int height, int depth) {
  maze->dim[0] = width;
  maze->dim[1] = height;
  maze->dim[2] = depth;
  maze->cost = 0;
  initPath(&maze->path);
  memset(maze->cell, 0, MAXDIM*MAXDIM*MAXDIM*sizeof(int));
}

void getPosition(Maze maze, Position *pos) {
  copyPosition(pos, maze.pos);
}

void setGoal(Position g, Maze *maze) {
  maze->goal = g;
}

void getGoal(Maze maze, Position *goal) {
  copyPosition(goal, maze.goal);
}

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
    case DOWN : maze->cell[x][y][z-1] |=    UP; break;
  }
}

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

int canMoveTo(Maze maze, Direction dir) {
  return (maze.cell[maze.pos.x][maze.pos.y][maze.pos.z] & dir ? 1 :0);
}

void printPossibleMoves(Maze maze) {
  Direction dir[6]={UP, DOWN, NORTH, EAST, SOUTH, WEST};
  int i;

  printf("Op positie ");
  printPosition(maze.pos);
  printf(" geldt:\n");
  for (i=0; i<6; i++) {
    printf("   %7s is ", convertDirectionToString(dir[i]));
    if (!canMoveTo(maze, dir[i])) {
      printf("NIET ");
    }
    printf("toegankelijk.\n");
  }
}

int isSolved(Maze maze) {
  return isEqualPosition(maze.pos, maze.goal);
}

int makeMove(Maze *maze, Direction dir) {
  if (!canMoveTo(*maze, dir)) {
    return 0;  /* FALSE==0 */
  }
  switch (dir) {
    case NORTH: maze->pos.y++; break;
    case SOUTH: maze->pos.y--; break;
    case WEST : maze->pos.x--; break;
    case EAST : maze->pos.x++; break;
    case UP   : maze->pos.z++; break;
    case DOWN : maze->pos.z--; break;
  }
  /* increment cost (count number of moves) */
  maze->cost++;

  /* add new location to path */
  appendPositionToPath(&maze->path, maze->pos);
  
  return 1;  /* TRUE==1 */
}

/* Sets the current state of the maze corresponsing to state */
void setMazeState(Maze *maze, State state) {
  copyPosition(&maze->pos, state.pos);
  copyPath(&maze->path, state.path);
  maze->cost = state.cost;
}

/* Gets the current state of the maze and returns it in state */
void getMazeState(State *state, Maze maze) {
  copyPosition(&state->pos, maze.pos);
  copyPath(&state->path, maze.path);
  state->cost = maze.cost;
}

