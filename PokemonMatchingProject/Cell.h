#pragma once
#include <iostream>
#include <utility>
#include <algorithm>
#include <math.h>

#define DELETED -1
#define NORMAL 0
#define SELECTED 1 // 3 state of the cells

using namespace std;

struct Cell {
	int x;
	int y;
	int state = NORMAL;
	int animal = 0;
	char bg[3][5];
	
	Cell();
};