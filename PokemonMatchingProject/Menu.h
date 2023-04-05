<<<<<<< HEAD
#pragma once
#include <iostream>
#include <string>
#include "System.h"

using namespace std;

struct Menu {
	static void menuInOrder(string[], int, int, int);
	static void printCenterText(string, int);
	static void drawBG(int, int);
	static void mainMenu();
	static void leaderboardsMenu();
	static void playerModes();
	static void gameModes();
=======
#pragma once
#include <iostream>
#include <string>
#include "System.h"

using namespace std;

struct Menu {
	static void menuInOrder(string[], int, int, int);
	static void printCenterText(string, int);
	static void drawBG(int, int);
	static void mainMenu();
	static void leaderboardsMenu();
	static void playerModes();
	static void gameModes();
>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
};