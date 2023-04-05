<<<<<<< HEAD
#pragma once

#include <iostream>
#include <fstream>
#include "Menu.h"
#include "Board.h"
#include "System.h"
#include "Player.h"

#define SAVE_FILE "profile.bin"
#define SAVE_FILE2 "profile2.bin"

using namespace std;

struct Game {
	static string State[11];
	static string gameMode[4];
	static string getProfile[2];
	static void inGame();
	static void savePlayerInfoToFile(Player, string);
	static vector<Player> readAllPlayerInfoFromFile(string);
	static void modifyPlayerInfoInFile(int, Player, string);
};
=======
#pragma once

#include <iostream>
#include <fstream>
#include "Menu.h"
#include "Board.h"
#include "System.h"
#include "Player.h"

#define SAVE_FILE "profile.bin"
#define SAVE_FILE2 "profile2.bin"

using namespace std;

struct Game {
	static string State[11];
	static string gameMode[4];
	static string getProfile[2];
	static void inGame();
	static void savePlayerInfoToFile(Player, string);
	static vector<Player> readAllPlayerInfoFromFile(string);
	static void modifyPlayerInfoInFile(int, Player, string);
};
>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
