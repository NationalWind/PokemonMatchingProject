<<<<<<< HEAD
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

=======
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

>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
