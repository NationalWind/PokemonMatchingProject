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
					 /_/___\___/_/\_\\__/_/_/_/\___/_//_/
					  / ___/__  ___  ___  ___ ____/ /_   
					 / /__/ _ \/ _ \/ _ \/ -_) __/ __/   
					 \___/\___/_//_/_//_/\__/\__/\__/    
                                    
                                      
			)";
}

void Menu::mainMenu() {
	System::setBGandText(BRIGHT_WHITE, BLACK);
	System::clearScreen();
	Menu::drawBG(BRIGHT_WHITE, RED);
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
	System::drawRectangle2(20, 10, 80, 15);
}
void Menu::playerModes() {
	System::clearScreen();
	printCenterText("Please choose the number of players", 10);
}

void Menu::gameModes() {
	System::clearScreen();
	printCenterText("Please choose your game level", 10);
}