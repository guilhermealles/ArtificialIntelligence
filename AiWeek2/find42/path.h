#ifndef PATH_H
#define PATH_H

#define MAXPATH 100000
#include "state.h"

typedef struct Path{
	  int length;
	  State path[MAXPATH];
}Path;
#endif