#include "direction.h"
#include <string.h>
static char dirstr[6];

char *convertDirectionToString(Direction dir) {
	switch (dir) {
		case NORTH: strcpy(dirstr, "NORTH"); break;
		case SOUTH: strcpy(dirstr, "SOUTH"); break;
		case WEST : strcpy(dirstr, "WEST"); break;
		case EAST : strcpy(dirstr, "EAST"); break;
		case UP   : strcpy(dirstr, "UP"); break;
		case DOWN : strcpy(dirstr, "DOWN"); break;
	}
	return dirstr;
}
