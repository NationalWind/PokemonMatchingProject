#pragma once

#include <iostream>
#include <fstream>
#include <conio.h>
#include "Menu.h"
#include "Board.h"
#include "System.h"
#include "Player.h"

#define SAVE_FILE "profile.bin"
#define SAVE_FILE1 "profile1.bin"
#define SAVE_FILE2 "profile2.bin"

#define BG_FILE "cabygon.txt"

using namespace std;

struct Game {
	static string State[11];
	static string gameMode[4];
	static string getProfile[2];
	static string gameBG[18];
	static void inGame();
	static void savePlayerInfoToFile(Player, string);
	static vector<Player> readAllPlayerInfoFromFile(string);
	static void modifyPlayerInfoInFile(int, Player, string);
};
