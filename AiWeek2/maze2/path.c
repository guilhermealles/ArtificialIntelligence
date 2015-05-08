#include <stdlib.h>
#include <string.h>
#include "path.h"

void initPath(Path *path) {
  path->length = 0;
  path->free = 1;
  path->positions = malloc(sizeof(Position));
}

static void resizePath(Path *path) {
  if (path->free == 0) {
    path->free = path->length;
    path->length = path->length;
    path->positions = realloc(path->positions,
      2*path->length*sizeof(Position));
  } else {
    if ((path->free > 1) && (path->free >= 2*path->length)) {
      path->positions = realloc(path->positions,
        (path->length+path->free)*sizeof(Position)/2);
      path->free = (path->length+path->free)/2 - path->length;
    }
  }
}

void appendPositionToPath(Path *path, Position pos) {
  resizePath(path);
  copyPosition(&path->positions[path->length], pos);
  path->length++;
  path->free--;
}

void removeLastPositionFromPath(Path *path) {
  if (path->length > 0) {
    path->length--;
    path->free++;
    resizePath(path);
  }
}

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

void printPathWithNewline(Path path) {
  printPath(path);
  printf("\n");
}

/* copy a path */
void copyPath(Path *dest, Path source) {
  int len = source.length;
  dest->length = len;
  dest->free = 0;
  dest->positions = malloc(len*sizeof(Position));
  memcpy(dest->positions, source.positions, len*sizeof(Position));
}

