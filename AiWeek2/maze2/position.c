#include "position.h"
#include "misc.h"

/* Read a position from file f. The result is placed in p.
 * the function reaturns 1 on succes, 0 otherwise.
 */
void readPositionFromFile(FILE *f, Position *p) {
  if (fscanf(f, "%d %d %d", &p->x, &p->y, &p->z) != 3) {
    error("Error: failed to read position from file.\n");
  }
}

/* Set the position of p to (x,y,z), i.e. implements p=(x,y,z). */
void setPosition(Position *p, int x, int y, int z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

void copyPosition(Position *p, Position q) {
  p->x = q.x;
  p->y = q.y;
  p->z = q.z;
}

int isEqualPosition(Position p, Position q) {
  return ((p.x==q.x) && (p.y==q.y) && (p.z==q.z));
}

int isEqualPosition2(Position p, int x, int y, int z) {
  return ((p.x==x) && (p.y==y) && (p.z==z));
}

void printPosition(Position p) {
  printf ("(%d,%d,%d)", p.x, p.y, p.z);
}

void printPositionWithNewline(Position p) {
  printf ("(%d,%d,%d)\n", p.x, p.y, p.z);
}
