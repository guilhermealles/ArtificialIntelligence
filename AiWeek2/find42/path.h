#ifndef PATH_H
#define PATH_H

#define MAXPATH 100000

typedef struct {
	int path_length;
	int values[MAXPATH];
	char operations[MAXPATH][3];
} Path;

void initPath(Path *path);
void appendToPath(Path *path, int value, char* operation);
void printPath(Path path);

#endif