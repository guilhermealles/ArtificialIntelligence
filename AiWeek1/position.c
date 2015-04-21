#include "position.h"
#include "misc.h"

/* Read a position from file f. The result is placed in p.
 * The function returns 1 on success, 0 otherwise.
 */
void readPositionFromFile(FILE *f, Position *p) {
  if (fscanf(f, "%d %d %d", &p->x, &p->y, &p->z) != 3) {
    error("Error: failed to read position from file.\n");
  }
}

/* Assign (x,y,z)  to p */
void setPosition(Position *p, int x, int y, int z) {
  p->x = x;
  p->y = y;
  p->z = z;
}

/* implements assignment p=q */
void copyPosition(Position *p, Position q) {
  p->x = q.x;
  p->y = q.y;
  p->z = q.z;
}

/* returns true(1) if p==q, otherwise false(0) */
int isEqualPosition(Position p, Position q) {
  return ((p.x==q.x) && (p.y==q.y) && (p.z==q.z));
}

/* returns true(1) if p==(x,y,z),  otherwise false(0) */
int isEqualPosition2(Position p, int x, int y, int z) {
  return ((p.x==x) && (p.y==y) && (p.z==z));
}

/* print the coordinate p on the screen */
void printPosition(Position p) {
  printf ("(%d,%d,%d)", p.x, p.y, p.z);
}
/* like printPosition, followed by a newline */
void printPositionWithNewline(Position p) {
  printf ("(%d,%d,%d)\n", p.x, p.y, p.z);
}
