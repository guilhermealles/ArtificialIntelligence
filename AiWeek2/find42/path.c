#include "path.h"
#include <stdio.h>
#include <string.h>

void initPath(Path *path) {
	path->values[0] = 0;
	strcpy(path->operations[0], "\0");
	path->path_length = 1;
}

void appendToPath(Path *path, int value, char *operation) {
	path->values[path->path_length] = value;
	path->operations[path->path_length][0] = operation[0];
	path->operations[path->path_length][1] = operation[1];
	path->operations[path->path_length][2] = '\0';
	path->path_length++;
}

void printPath(Path path) {
	printf("%d ", path.values[0]);

	for(int i = 1; i < path.path_length; i++)
  	{
    	printf(" (%s) -> ", path.operations[i]);
    	printf("%d ", path.values[i]);
  	}
}