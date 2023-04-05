<<<<<<< HEAD
#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include "Cell.h"
#include "System.h"


using namespace std;


struct Board {
	// board size
	int height;
	int width;

	// the board's topleft corner coordinate
	int boardX;
	int boardY;

	pair <int, int> saveAngle;
	pair <int, int> saveCorner1;
	pair <int, int> saveCorner2;

	Cell** board;
	Board(int, int, int, int);
	Board();
	~Board();

	//void initBoard(int, int, int, int);
	int getAnimal(pair<int, int>);
	int getState(pair<int, int>);
	pair <int, int> getCoord(pair<int, int>);
	void DrawLine_I(pair<int, int>, pair<int, int>);
	void DrawLine_L(pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_Z(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_U(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);

	void createBoard(pair <int, int>, pair <bool, pair <pair <int, int>, pair <int, int>>>);

	bool CheckLineI(pair<int, int>, pair<int, int>);
	bool CheckLineL(pair<int, int>, pair<int, int>);
	bool CheckLineZ(pair<int, int>, pair<int, int>);
	bool CheckLineU(pair<int, int>, pair<int, int>);



=======
#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include "Cell.h"
#include "System.h"


using namespace std;


struct Board {
	// board size
	int height;
	int width;

	// the board's topleft corner coordinate
	int boardX;
	int boardY;

	pair <int, int> saveAngle;
	pair <int, int> saveCorner1;
	pair <int, int> saveCorner2;

	Cell** board;
	Board(int, int, int, int);
	Board();
	~Board();

	//void initBoard(int, int, int, int);
	int getAnimal(pair<int, int>);
	int getState(pair<int, int>);
	pair <int, int> getCoord(pair<int, int>);
	void DrawLine_I(pair<int, int>, pair<int, int>);
	void DrawLine_L(pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_Z(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_U(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);

	void createBoard(pair <int, int>, pair <bool, pair <pair <int, int>, pair <int, int>>>);

	bool CheckLineI(pair<int, int>, pair<int, int>);
	bool CheckLineL(pair<int, int>, pair<int, int>);
	bool CheckLineZ(pair<int, int>, pair<int, int>);
	bool CheckLineU(pair<int, int>, pair<int, int>);



>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
};