#pragma once
#include <iostream>
#include "System.h"
#include "Board.h"
#include <limits>
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

struct Player {
	char playerName[15];
	char playerPassword[15];
	int playerID;
	int playerScore;
	int playerMode;
	Cell playerBoard[8][10];
	Player();
	void inputProfile(int);
};

