#include "Cell.h"

Cell::Cell() {
	x = 0;
	y = 0;
	state = NORMAL;
	animal = 0;
	memset(bg, 0, sizeof(bg));
}