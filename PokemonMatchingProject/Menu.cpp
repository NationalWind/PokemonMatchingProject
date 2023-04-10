#include "Menu.h"

void Menu::menuInOrder(string State[], int currSelection, int y, int maxSelecetion) {
	int currOption = -1;
	while (currOption < maxSelecetion) {
		currOption++;
		y++;
		if (currOption == currSelection)
			System::setBGandText(YELLOW, BLUE);
		printCenterText(State[currOption], y);
		System::setBGandText(BRIGHT_WHITE, BLACK);
	}
}


void Menu::printCenterText(string text, int y) {
	int x = (120 - text.size()) / 2;
	System::gotoXY(x, y);
	cout << text;
}


void Menu::drawBG(int colorBG, int colorText) {
	System::gotoXY(38, 5);
	System::setBGandText(colorBG, colorText);
	cout << R"(	
			   ___       __                     
			  / _ \___  / /_____ __ _  ___  ___ 
			 / ___/ _ \/  '_/ -_)  ' \/ _ \/ _ \
			/_/   \___/_/\_\\__/_/_/_/\___/_//_/                         

	  _____                       __ 
	 / ___/__  ___  ___  ___ ____/ /_
	/ /__/ _ \/ _ \/ _ \/ -_) __/ __/
	\___/\___/_//_/_//_/\__/\__/\__/ 
                                 

			)";
}

void Menu::mainMenu() {
	System::setBGandText(BRIGHT_WHITE, BLACK);
	System::clearScreen();
	Menu::drawBG(BRIGHT_WHITE, BLUE);
	System::setBGandText(BRIGHT_WHITE, RED);
	Menu::drawDragon();
	System::setBGandText(BRIGHT_WHITE, BLACK);
}

void Menu::leaderboardsMenu() {
	System::gotoXY(38, 5);
	System::setBGandText(BRIGHT_WHITE, RED);
	cout << R"(	
				   __               __        __                    __  
				  / /  ___ ___ ____/ /__ ____/ /  ___  ___ ________/ /__
				 / /__/ -_) _ `/ _  / -_) __/ _ \/ _ \/ _ `/ __/ _  (_-<
				/____/\__/\_,_/\_,_/\__/_/ /_.__/\___/\_,_/_/  \_,_/___/
                                                                              
			)";
	System::setBGandText(BRIGHT_WHITE, BLACK);
	System::drawRectangle2(10, 10, 100, 15);
}

void Menu::instructionsMenu() {
	System::gotoXY(38, 5);
	System::setBGandText(BRIGHT_WHITE, RED);
	cout << R"(	
					  ___         _               _   _             
					 |_ _|_ _  __| |_ _ _ _  _ __| |_(_)___ _ _  ___
					  | || ' \(_-<  _| '_| || / _|  _| / _ \ ' \(_-<
					 |___|_||_/__/\__|_|  \_,_\__|\__|_\___/_||_/__/
                                                
			)";
	System::setBGandText(BRIGHT_WHITE, BLACK);
	System::drawRectangle2(50, 10, 30, 15);
}

void Menu::playerModes() {
	System::clearScreen();
	printCenterText("Please decide...", 10);
}

void Menu::gameModes() {
	System::clearScreen();
	printCenterText("Please choose your game level", 10);
}

void Menu::drawDragon() {
	ifstream dragon("bigdragon.txt");
	string s;
	int y = 2;
	while (getline(dragon, s)) {
		System::gotoXY(65, y++);
		cout << s;
	}
	dragon.close();
}