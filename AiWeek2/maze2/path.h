#ifndef PATH_H
#define PATH_H

#include "position.h"

/* Implements the data type Path, a series of Positions. */
typedef struct Path {
  int length;   /* length of the path */
  int free;     /* number of available positions (until realloc) */
  Position *positions;
} Path;

/* initialize path (make empty path) */
void initPath(Path *path);

/* append pos to the end of the path */
void appendPositionToPath(Path *path, Position pos);

/* remove last position from path */
void removeLastPositionFromPath(Path *path);

/* print path on the screen */
void printPath(Path path);

/* print path on the screen followed by a newline */
void printPathWithNewline(Path path);

/* copy a path */
void copyPath(Path *dest, Path source);

#endif
