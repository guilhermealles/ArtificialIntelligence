#include <stdlib.h>
#include "path.h"

/* initialize path (make empty path) */
void initPath(Path *path) {
  path->length = 0;
  path->free = 1;
  path->positions = malloc(sizeof(Position));
}

static void resizePath(Path *path) {
  if (path->free == 0) {
    path->free = path->length;
    path->length = 2*path->length;
    path->positions = realloc(path->positions,
      path->length*sizeof(Position));
  } else {
    if (path->free >= 2*path->length) {
      path->positions = realloc(path->positions,
        (path->length+path->free)*sizeof(Position)/2);
      path->free = (path->length+path->free)/2 - path->length;
    }
  }
}

/* append pos to the end of the path */
void appendPositionToPath(Path *path, Position pos) {
  resizePath(path);
  copyPosition(&path->positions[path->length], pos);
  path->length++;
  path->free--;
}

/* remove last position from path */
void removeLastPositionFromPath(Path *path) {
  if (path->length > 0) {
    path->length--;
    path->free++;
    resizePath(path);
  }
}

/* print path on the screen */
void printPath(Path path) {
  int i;
  if (path.length > 0) {
    printPosition(path.positions[0]);
    for (i=1; i<path.length; i++) {
      printf("->");
      printPosition(path.positions[i]);
    }
  }
}

/* print path on the screen followed by a newline */
void printPathWithNewline(Path path) {
  printPath(path);
  printf("\n");
}
