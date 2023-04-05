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
};