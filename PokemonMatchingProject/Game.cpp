#include "Game.h"


string Game::State[11] = { "Play", "Leaderboards", "Instructions", "Exit", "Settings", "Back",
								"Are you ready?", "Level 3", "Level 1", "Level 2", "Main Menu"};

string Game::gameMode[4] = { "1 Player Easy", "1 Player Hard", "2 Players Easy", "2 Players Hard" };

string Game::getProfile[2] = {"Profile 1", "Profile 2"};

string Game::gameBG[18] = {};


void Game::savePlayerInfoToFile(Player player, string filename) {
	ofstream outFile(filename, ios::binary | ios::app);
	outFile.write((char*)&player, sizeof(player));
	outFile.close();
}

void Game::modifyPlayerInfoInFile(int playerId, Player newPlayerInfo, string filename) {
	fstream file(filename, ios::in | ios::out | ios::binary);
	if (file) {
		file.seekg(0, ios::end);
		int fileSize = file.tellg();
		int numPlayers = fileSize / sizeof(Player);
		file.seekg(0, ios::beg);
		for (int i = 0; i < numPlayers; i++) {
			Player player;
			file.read((char*)&player, sizeof(player));
			if (player.playerID == playerId) {
				file.seekp(i * sizeof(Player), ios::beg);
				file.write((char*)&newPlayerInfo, sizeof(newPlayerInfo));
				break;
			}
		}
		file.close();
	}
}

vector<Player> Game::readAllPlayerInfoFromFile(string filename) {
	ifstream inFile(filename, ios::binary);
	vector<Player> players;
	if (inFile) {
		inFile.seekg(0, ios::end);
		int fileSize = inFile.tellg();
		inFile.seekg(0, ios::beg);
		int numPlayers = fileSize / sizeof(Player);
		for (int i = 0; i < numPlayers; i++) {
			Player player;
			inFile.read((char*)&player, sizeof(player));
			players.push_back(player);
		}
		inFile.close();
	}
	return players;
}


void getBackground(string filename) {
	ifstream bg(filename);
	for (int i = 0; i < 18; i++) {
		getline(bg, Game::gameBG[i]);
	}
	bg.close();
}

// save the characters for shuffling
vector <int> generateAnimal() {
	vector <int> tmp; 
	vector <bool> mark(26, false);
	int sum = 0;
	for (int i = 1; i <= 50; i++) {
		int c = System::randBetween(65, 90);
		while (mark[c - 65])
			c = System::randBetween(65, 90);
		mark[c - 65] = true;
		int num = System::randBetween(1, 3);
		if (sum + num * 2 > 48)
			break;
		sum += num * 2;
		for (int j = 1; j <= num * 2; j++)
			tmp.push_back(c);
	}
	if (sum < 48) {
		int c = System::randBetween(65, 90);
		while (mark[c - 65])
			c = System::randBetween(65, 90);
		mark[c - 65] = true;
		for (int j = 1; j <= 48 - sum; j++)
			tmp.push_back(c);
	}
	return tmp;
}

void shuffleBoard(Board* &b, vector <int>& tmp) {
	random_device rd;
	mt19937 g(rd());
	shuffle(tmp.begin(), tmp.end(), g);
	int cnt = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++) {
			RemoveHead(b->boardList[i]);
		}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 10; j++) {
			Cell cell;
			int state;
			int ani;
			if (i == 0 || i == 7 || j == 0 || j == 9) {
				state = DELETED;
				ani = 0;
			}
			else {
				state = NORMAL;
				ani = tmp[cnt++];
			}
			cell.animal = ani;
			cell.state = state;
			cell.x = b->boardX + j * 5;
			cell.y = b->boardY + i * 3;
			AddTail(b->boardList[i], cell);
			b->board[i][j].animal = ani;
			b->board[i][j].state = state;
			b->board[i][j].x = b->boardX + j * 5;
			b->board[i][j].y = b->boardY + i * 3;
		}
	}
}

void resetBoard(Board* &b) {
	vector <int> tmp;
	for (int i = 1; i <= 6; i++)
		for (int j = 1; j <= 8; j++)
			if (b->board[i][j].state == NORMAL)
				tmp.push_back(b->board[i][j].animal);
	random_device rd;
	mt19937 g(rd());
	shuffle(tmp.begin(), tmp.end(), g);
	int cnt = 0;
	for (int i = 1; i <= 6; i++)
		for (int j = 1; j <= 8; j++)
			if (b->board[i][j].state == NORMAL)
				b->board[i][j].animal = tmp[cnt++];

	vector <int> tmps;
	for (int i = 1; i <= 6; i++) {
		Node* p = b->boardList[i];
		while (p) {
			Cell cell = p->data;
			tmps.push_back(cell.animal);
			p = p->pNext;
		}
	}
	shuffle(tmps.begin(), tmps.end(), g);
	cnt = 0;
	for (int i = 1; i <= 6; i++) {
		Node* p = b->boardList[i];
		while (p) {
			p->data.animal = tmps[cnt++];
			p = p->pNext;
		}
	}
}

void convertTime(int seconds, int& hours, int& minutes, int& remainingSeconds) {
	hours = seconds / 3600;
	remainingSeconds = seconds % 3600;
	minutes = remainingSeconds / 60;
	remainingSeconds = remainingSeconds % 60;
}

void drawEscBoard() {
	System::drawRectangle(0, 0, 30, 3);
	System::gotoXY(1, 1);
	cout << "[Esc]: Exit/Turn back";
}

void drawScoreBoard(Player p){
	System::drawRectangle(70, 20, 22, 7);
	System::gotoXY(71, 21);
	cout << "Player: " << p.playerName;
	System::gotoXY(71, 23);
	cout << "Score: " << p.playerScore;
	System::gotoXY(71, 25);
	cout << "[H]: Move Suggestion";
}

void drawTimeBoard(Player p) {
	int hours, minutes, seconds;
	convertTime(p.playerTime, hours, minutes, seconds);
	cout << hours << ":" << minutes << ":" << seconds;
}

void countDown() {
	System::clearScreen();
	Menu::printCenterText("Game started in...", 3);
	System::gotoXY(40, 5);
	cout << R"(

							  ____  
							 |___ \ 
							   __) |
							  |__ < 
							  ___) |
							 |____/ 
        
        

)";
	Sleep(1000);
	System::gotoXY(40, 13);
	cout << R"(

							  ___  
							 |__ \ 
                                                            ) |
							   / / 
							  / /_ 
							 |____|
       
       

)";
	Sleep(1000);
	System::gotoXY(40, 20);
	cout << R"( 

							  __ 
							 /_ |
							  | |
							  | |
							  | |
							  |_|
)";
	Sleep(1000);
}

void drawWinning() {
	System::clearScreen();
	System::gotoXY(0, 12);
	System::setBGandText(BRIGHT_WHITE, YELLOW);
	System::setBGandText(BRIGHT_WHITE, YELLOW);
	cout << R"(
				____   ____.___ ____  __.___________________ _______________.___.
				\   \ /   /|   |    |/ _|\__    ___/\_____  \\______   \__  |   |
				 \   Y   / |   |      <    |    |    /   |   \|       _//   |   |
				  \     /  |   |    |  \   |    |   /    |    \    |   \\____   |
				   \___/   |___|____|__ \  |____|   \_______  /____|_  // ______|
									   \/                   \/       \/ \/       
	)";
	System::setBGandText(BRIGHT_WHITE, BLACK);
}

bool drawAskBoard() {
	System::clearScreen;
	Menu::printCenterText("Do you want to play this mode again?", 10);
	bool currAsk = 1;
	int k = 0;
	do {
		System::gotoXY(57, 11);
		if (currAsk)
			System::setBGandText(YELLOW, BLUE);
		else
			System::setBGandText(BRIGHT_WHITE, BLUE);
		cout << "YES";
		System::gotoXY(61, 11);
		if (currAsk)
			System::setBGandText(BRIGHT_WHITE, BLACK);
		else
			System::setBGandText(YELLOW, BLUE);
		cout << "NO";
		k = System::getKeyboard();
		if (k == 3 || k == 4 || k == 7 || k == 8)
			currAsk = !currAsk;
	} while (k != 10);
	System::setBGandText(BRIGHT_WHITE, BLACK);
	return currAsk;
}

bool cmpPlayer(Player a, Player b) {
	if (a.playerScore != b.playerScore)
		return a.playerScore > b.playerScore;
	else
		return a.playerTime < b.playerTime;
}

void Game::inGame() {
	srand(time(nullptr));
	string currState = State[10];
	bool playing = true; // main game logic
	int currMenuSelection = 0; // main menu counter
	int currPlayerSelection = 0; // player menu counter
	int currGameSelection = 0; // game menu counter
	int gameChoosing = 0; // game mode
	clock_t startTime = clock(); // timer start
	int currTime; // save Time;
	pair <int, int> currCell = { 1, 1 };
	pair <bool, pair <pair <int, int>, pair <int, int>>> currSuggested = { 0, {{0, 0}, {0, 0}} }; // move suggestion: check if is suggested, 2 suggested cell
	//load previous data
	vector <Player> players;
	players = readAllPlayerInfoFromFile(SAVE_FILE);
	sort(players.begin(), players.end(), cmpPlayer);
	vector <Player> players1;
	players1 = readAllPlayerInfoFromFile(SAVE_FILE1);
	sort(players1.begin(), players1.end(), cmpPlayer);
	vector <Player> players2;
	players2 = readAllPlayerInfoFromFile(SAVE_FILE2);
	sort(players2.begin(), players2.end(), cmpPlayer);
	Board *b = new Board(8, 10, 5, 5);
	getBackground(BG_FILE);
	// Cell: 3x5
	// Board: 6x8 --> real: 8x10
	vector<pair<int, int>>CurrSelectedCell;
	Player player1;
	Player tmpPlayer;
	System::playSound(0);
	while (playing) {
		//mainMenu
		if (currState == State[10]) {
			Menu::mainMenu();
			Menu::menuInOrder(State, currMenuSelection, 20, 3);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currMenuSelection++;
				if (currMenuSelection > 3)
					currMenuSelection -= 4;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currMenuSelection--;
				if (currMenuSelection < 0)
					currMenuSelection += 4;
			}
			if (k == 10) {
				System::playSound(2);
				currState = State[currMenuSelection];
			}
			if (k == 1)
				playing = 0;
		}
		// choose number of players menu
		if (currState == State[0]) {
			Menu::playerModes();
			string menuPlayer[2] = { State[6], State[5] };
			Menu::menuInOrder(menuPlayer, currPlayerSelection, 20, 1);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currPlayerSelection++;
				if (currPlayerSelection > 1)
					currPlayerSelection -= 2;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currPlayerSelection--;
				if (currPlayerSelection < 0)
					currPlayerSelection += 2;
			}
			if (k == 10) {
				System::playSound(2);
				if (currPlayerSelection == 1)
					currState = State[10];
				else
					currState = menuPlayer[currPlayerSelection];
			}
		}
		// Instructions
		if (currState == State[2]) {
			System::clearScreen();
			drawEscBoard();
			Menu::instructionsMenu();
			int up = 30;
			int down = 31;
			int left = 17;
			int right = 16;
			System::gotoXY(56, 12);
			cout << "Up: [W] or [" << (char)up << "]";
			System::gotoXY(56, 14);
			cout << "Down: [S] or [" << (char)down << "]";
			System::gotoXY(56, 16);
			cout << "Left: [A] or [" << (char)left << "]";
			System::gotoXY(56, 18);
			cout << "Right: [D] or [" << (char)right << "]";
			System::gotoXY(56, 20);
			cout << "Move Suggestion: [H]";
			int k = System::getKeyboard();
			if (k == 1) {
				System::playSound(2);
				currState = State[10];
			}
		}
		// Leaderboards
		if (currState == State[1]) {
			System::clearScreen();
			drawEscBoard();
			Menu::leaderboardsMenu();
			System::gotoXY(12, 12);
			cout << "No.";
			System::gotoXY(20, 12);
			cout << "Player (Lv1)";
			System::gotoXY(35, 12);
			cout << "Score";
			System::gotoXY(42, 12);
			cout << "Time";
			int playerY = 15, cnt = 0;
			for (auto &player : players) {
				System::gotoXY(12, playerY);
				cout << ++cnt << ".";
				System::gotoXY(20, playerY);
				cout << player.playerName;
				System::gotoXY(35, playerY);
				cout << player.playerScore;
				System::gotoXY(42, playerY);
				drawTimeBoard(player);
				playerY += 2;
				if (cnt == 5)
					break;
			}
			System::gotoXY(50, 12);
			cout << "Player (Lv2)";
			System::gotoXY(65, 12);
			cout << "Score";
			System::gotoXY(72, 12);
			cout << "Time";
			playerY = 15, cnt = 0;
			for (auto& player : players1) {
				cnt++;
				System::gotoXY(50, playerY);
				cout << player.playerName;
				System::gotoXY(65, playerY);
				cout << player.playerScore;
				System::gotoXY(72, playerY);
				drawTimeBoard(player);
				playerY += 2;
				if (cnt == 5)
					break;
			}
			System::gotoXY(80, 12);
			cout << "Player (Lv3)";
			System::gotoXY(95, 12);
			cout << "Score";
			System::gotoXY(102, 12);
			cout << "Time";
			playerY = 15, cnt = 0;
			for (auto& player : players2) {
				cnt++;
				System::gotoXY(80, playerY);
				cout << player.playerName;
				System::gotoXY(95, playerY);
				cout << player.playerScore;
				System::gotoXY(102, playerY);
				drawTimeBoard(player);
				playerY += 2;
				if (cnt == 5)
					break;
			}
			int k = System::getKeyboard();
			if (k == 1) {
				System::playSound(2);
				currState = State[10];
			}
		}
		// Player game mod choosing
		if (currState == State[6]) {
			Menu::gameModes();
			string menuPlayer[4] = { State[8], State[9], State[7], State[5] };
			Menu::menuInOrder(menuPlayer, currGameSelection, 20, 3);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currGameSelection++;
				if (currGameSelection > 3)
					currGameSelection -= 4;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currGameSelection--;
				if (currGameSelection < 0)
					currGameSelection += 4;
			}
			if (k == 10)
			{
				System::playSound(2);
				if (currGameSelection == 3)
					currState = State[0];
				else {
					player1.playerMode = gameChoosing = currGameSelection;
					currState = getProfile[0];
				}
			}
		}
		//info 1 player
		if (currState == getProfile[0]) {
			bool registered = 0;
			do {
				System::clearScreen();
				System::drawRectangle2(18, 14, 35, 10);
				Menu::printCenterText("Player Profile", 10);
				System::gotoXY(20, 15);
				cout << "Username: ";
				cin >> player1.playerName;
				if (player1.playerMode == 0) {
					for (auto& player : players) {
						if (strcmp(player.playerName, player1.playerName) == 0) {
							tmpPlayer = player;
							registered = 1;
							break;
						}
					}
				}
				else {
					for (auto& player : players1) {
						if (strcmp(player.playerName, player1.playerName) == 0) {
							tmpPlayer = player;
							registered = 1;
							break;
						}
					}
				}
				System::gotoXY(20, 18);
				cout << "Password: ";
				cin >> player1.playerPassword;
				if (registered) {
					if (strcmp(tmpPlayer.playerPassword, player1.playerPassword) != 0) {
						registered = 0;
						System::gotoXY(20, 22);
						cout << "Wrong Password!";
						Sleep(500);
					}
					else {
						player1 = tmpPlayer;
						for (int i = 0; i < 8; i++)
							for (int j = 0; j < 10; j++) {
								b->board[i][j] = tmpPlayer.playerBoard[i][j];
								AddTail(b->boardList[i], tmpPlayer.playerBoard[i][j]);
							}
						System::gotoXY(20, 22);
						System::playSound(2);
						cout << "Welcome Back <3";
						Sleep(1000);
					}
				}
				else {
					System::gotoXY(20, 22);
					player1.playerID = System::randBetween(10000, 99999);
					if (player1.playerMode == 0)
						savePlayerInfoToFile(player1, SAVE_FILE);
					else if (player1.playerMode == 1)
						savePlayerInfoToFile(player1, SAVE_FILE1);
					else if (player1.playerMode == 2)
						savePlayerInfoToFile(player1, SAVE_FILE2);
					cout << "Welcome New Player :3";
					System::playSound(2);
					Sleep(500);
					registered = 1;
					for (int i = 1; i <= 6; i++)
						for (int j = 1; j <= 8; j++)
							b->board[i][j].state = NORMAL;
					player1.playerScore = 0;
					player1.playerTime = 0;
					vector <int> tmp;
					tmp = generateAnimal();
					shuffleBoard(b, tmp);
				}
			} while (!registered);
			countDown();
			currTime = 0;
			startTime = clock();
			currState = gameMode[gameChoosing];
		}

		// Level 1
		if (currState == gameMode[0]) {
			System::clearScreen();
			drawEscBoard();
			drawScoreBoard(player1);
			System::drawRectangle2(70, 10, 10, 3);
			System::gotoXY(71, 11);
			drawTimeBoard(player1);
			b->createBoard(currCell, currSuggested, Game::gameBG);
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					player1.playerBoard[i][j] = b->board[i][j];
			modifyPlayerInfoInFile(player1.playerID, player1, SAVE_FILE);
			bool already = 0;
			for (auto& player : players) {
				if (player.playerID == player1.playerID) {
					already = 1;
					player = player1;
					break;
				}
			}
			if (!already) {
				players.push_back(player1);
				sort(players.begin(), players.end(), cmpPlayer);
			}
			bool check = 0;
			for (int i1 = 1; i1 <= 6; i1++) {
				for (int j1 = 1; j1 <= 8; j1++) {
					for (int i2 = 1; i2 <= 6; i2++) {
						for (int j2 = 1; j2 <= 8; j2++) {
							if (!check && make_pair(i1, j1) != make_pair(i2, j2) &&
								b->getAnimal(make_pair(i1, j1)) != 0 && b->getAnimal(make_pair(i2, j2)) != 0 &&
								b->getAnimal(make_pair(i1, j1)) == b->getAnimal(make_pair(i2, j2))) {
								vector <pair <int, int>> currTest;
								currTest.clear();
								currTest.push_back(make_pair(i1, j1));
								currTest.push_back(make_pair(i2, j2));
								System::gotoXY(0, 0);
								if (b->CheckLineI(currTest[0], currTest[1]) ||
									(b->CheckLineL(currTest[0], currTest[1])) ||
									b->CheckLineZ(currTest[0], currTest[1]) ||
									b->CheckLineU(currTest[0], currTest[1])) {
									check = 1;
								}
							}
						}
					}
				}
			}
			if (!check) {
				System::gotoXY(50, 18);
				cout << "No moves left!";
				resetBoard(b);
			}
			bool overOut = 0;
			if (_kbhit()) {
				int k = System::getKeyboard();
				if (k == 2 || k == 6) {
					System::playSound(5);
					currCell.first--;
					if (currCell.first < 1)
						currCell.first += 6;
				}
				else if (k == 3 || k == 7) {
					System::playSound(5);
					currCell.second--;
					if (currCell.second < 1)
						currCell.second += 8;
				}
				else if (k == 4 || k == 8) {
					System::playSound(5);
					currCell.second++;
					if (currCell.second > 8)
						currCell.second -= 8;
				}
				else if (k == 5 || k == 9) {
					System::playSound(5);
					currCell.first++;
					if (currCell.first > 6)
						currCell.first -= 6;
				}
				else if (k == 10) {
					System::playSound(2);
					currSuggested.first = 0;
					if (b->board[currCell.first][currCell.second].state == NORMAL) {
						b->board[currCell.first][currCell.second].state = SELECTED;
						CurrSelectedCell.push_back(currCell);
						if (CurrSelectedCell.size() == 2) {
							if ((b->getAnimal(CurrSelectedCell[0]) == b->getAnimal(CurrSelectedCell[1])) &&
								(b->CheckLineI(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									(b->CheckLineL(CurrSelectedCell[0], CurrSelectedCell[1])) ||
									b->CheckLineZ(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									b->CheckLineU(CurrSelectedCell[0], CurrSelectedCell[1]))) {
								player1.playerScore += 10;
								if (b->CheckLineI(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									b->DrawLine_I(block0, block1);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineL(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockAngle = b->getCoord(b->saveAngle);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockAngle.first += 2;
									blockAngle.second += 1;
									b->DrawLine_L(block0, block1, blockAngle);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineZ(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockCorner1 = b->getCoord(b->saveCorner1);
									pair <int, int> blockCorner2 = b->getCoord(b->saveCorner2);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_Z(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineU(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockCorner1 = b->getCoord(b->saveCorner1);
									pair <int, int> blockCorner2 = b->getCoord(b->saveCorner2);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_U(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								System::playSound(3);
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].state = DELETED;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].state = DELETED;
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].animal = 0;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].animal = 0;
								bool over = 1;
								for (int i = 1; i <= 6; i++)
									for (int j = 1; j <= 8; j++)
										if (b->getAnimal(make_pair(i, j)) != 0)
											over = 0;
								if (over) {
									drawWinning();
									System::playSound(6);
									overOut = 1;
									Sleep(10000);
									bool ask = drawAskBoard();
									if (ask) {
										vector <int> tmp = generateAnimal();
										shuffleBoard(b, tmp);
										currCell = { 1, 1 };
										currSuggested.first = 0;
										Sleep(2000);
									}
									else {
										currState = State[10];
									}
								}
							}
							else {
								System::playSound(4);
								player1.playerScore -= 5;
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].state = NORMAL;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].state = NORMAL;
							}
							CurrSelectedCell.clear();
						}
					}
					else if (b->board[currCell.first][currCell.second].state == SELECTED) {
						b->board[currCell.first][currCell.second].state = NORMAL;
						CurrSelectedCell.clear();
					}
				}
				else if (k == 11) {
					bool found = 0;
					player1.playerScore -= 2;
					System::playSound(2);
					for (int i1 = 1; i1 <= 6; i1++) {
						for (int j1 = 1; j1 <= 8; j1++) {
							for (int i2 = 1; i2 <= 6; i2++) {
								for (int j2 = 1; j2 <= 8; j2++) {
									if (!found && make_pair(i1, j1) != make_pair(i2, j2) &&
										b->getAnimal(make_pair(i1, j1)) != 0 && b->getAnimal(make_pair(i2, j2)) != 0 &&
										b->getAnimal(make_pair(i1, j1)) == b->getAnimal(make_pair(i2, j2))) {
										vector <pair <int, int>> currTest;
										currTest.clear();
										currTest.push_back(make_pair(i1, j1));
										currTest.push_back(make_pair(i2, j2));
										if (b->CheckLineI(currTest[0], currTest[1]) ||
											(b->CheckLineL(currTest[0], currTest[1])) ||
											b->CheckLineZ(currTest[0], currTest[1]) ||
											b->CheckLineU(currTest[0], currTest[1])) {
											found = 1;
											currSuggested = { found, {make_pair(i1, j1), make_pair(i2, j2)} };
										}
									}
								}
							}
						}
					}
				}
				else if (k == 1) {
					bool already = 0;
					for (auto& player : players) {
						if (player.playerID == player1.playerID) {
							already = 1;
							player = player1;
							break;
						}
					}
					if (!already) {
						players.push_back(player1);
						sort(players.begin(), players.end(), cmpPlayer);
					}
					currState = State[10];
				}
			}
			if (!overOut) {
				int nowTime = (clock() - startTime) / CLOCKS_PER_SEC;
				if (nowTime - currTime == 1) {
					currTime = nowTime;
					player1.playerTime++;
				}
			}
		}

		// Level 2
		// LinkedList
		if (currState == gameMode[1]) {
			System::clearScreen();
			drawEscBoard();
			drawScoreBoard(player1);
			System::drawRectangle2(70, 10, 10, 3);
			System::gotoXY(71, 11);
			drawTimeBoard(player1);
			b->createBoardList(currCell, currSuggested, Game::gameBG);
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					player1.playerBoard[i][j] = getNodeData(b->boardList[i], j);
			modifyPlayerInfoInFile(player1.playerID, player1, SAVE_FILE1);
			bool already = 0;
			for (auto& player : players1) {
				if (player.playerID == player1.playerID) {
					already = 1;
					player = player1;
					break;
				}
			}
			if (!already) {
				players1.push_back(player1);
				sort(players1.begin(), players1.end(), cmpPlayer);
			}
			bool check = 0;
			for (int i1 = 1; i1 <= 6; i1++) {
				for (int j1 = 1; j1 <= 8; j1++) {
					for (int i2 = 1; i2 <= 6; i2++) {
						for (int j2 = 1; j2 <= 8; j2++) {
							if (!check && make_pair(i1, j1) != make_pair(i2, j2) &&
								getNodeData(b->boardList[i1], j1).animal != 0 && getNodeData(b->boardList[i2], j2).animal != 0 &&
								getNodeData(b->boardList[i1], j1).animal == getNodeData(b->boardList[i2], j2).animal) {
								vector <pair <int, int>> currTest;
								currTest.clear();
								currTest.push_back(make_pair(i1, j1));
								currTest.push_back(make_pair(i2, j2));
								System::gotoXY(0, 0);
								if (b->CheckLineIList(currTest[0], currTest[1]) ||
									b->CheckLineLList(currTest[0], currTest[1]) ||
									b->CheckLineZList(currTest[0], currTest[1]) ||
									b->CheckLineUList(currTest[0], currTest[1])) {
									check = 1;
								}
							}
						}
					}
				}
			}
			if (!check) {
				System::gotoXY(50, 18);
				cout << "No moves left!";
				resetBoard(b);
			}
			bool overOut = 0;
			if (_kbhit()) {
				int k = System::getKeyboard();
				if (k == 2 || k == 6) {
					System::playSound(5);
					currCell.first--;
					if (currCell.first < 1)
						currCell.first += 6;
				}
				else if (k == 3 || k == 7) {
					System::playSound(5);
					currCell.second--;
					if (currCell.second < 1)
						currCell.second += 8;
				}
				else if (k == 4 || k == 8) {
					System::playSound(5);
					currCell.second++;
					if (currCell.second > 8)
						currCell.second -= 8;
				}
				else if (k == 5 || k == 9) {
					System::playSound(5);
					currCell.first++;
					if (currCell.first > 6)
						currCell.first -= 6;
				}
				else if (k == 10) {
					System::playSound(2);
					Cell cell = getNodeData(b->boardList[currCell.first], currCell.second);
					if (cell.state == NORMAL) {
						cell.state = SELECTED;
						editNodeData(b->boardList[currCell.first], currCell.second, cell);
						CurrSelectedCell.push_back(currCell);
						if (CurrSelectedCell.size() == 2) {
							currSuggested.first = 0;
							if ((getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).animal ==
								getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).animal) &&
								(b->CheckLineIList(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									(b->CheckLineLList(CurrSelectedCell[0], CurrSelectedCell[1])) ||
									b->CheckLineZList(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									b->CheckLineUList(CurrSelectedCell[0], CurrSelectedCell[1]))) {
								player1.playerScore += 10;
								if (b->CheckLineIList(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = make_pair(getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).x,
										getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).y);
									pair <int, int> block1 = make_pair(getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).x,
										getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).y);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									b->DrawLine_I(block0, block1);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineLList(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = make_pair(getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).x,
										getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).y);
									pair <int, int> block1 = make_pair(getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).x,
										getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).y);
									pair <int, int> blockAngle = make_pair(getNodeData(b->boardList[b->saveAngle.first], b->saveAngle.second).x,
										getNodeData(b->boardList[b->saveAngle.first], b->saveAngle.second).y);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockAngle.first += 2;
									blockAngle.second += 1;
									b->DrawLine_L(block0, block1, blockAngle);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineZList(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = make_pair(getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).x,
										getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).y);
									pair <int, int> block1 = make_pair(getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).x,
										getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).y);
									pair <int, int> blockCorner1 = make_pair(getNodeData(b->boardList[b->saveCorner1.first], b->saveCorner1.second).x,
										getNodeData(b->boardList[b->saveCorner1.first], b->saveCorner1.second).y);
									pair <int, int> blockCorner2 = make_pair(getNodeData(b->boardList[b->saveCorner2.first], b->saveCorner2.second).x,
										getNodeData(b->boardList[b->saveCorner2.first], b->saveCorner2.second).y);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_Z(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineUList(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = make_pair(getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).x,
										getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second).y);
									pair <int, int> block1 = make_pair(getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).x,
										getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second).y);
									pair <int, int> blockCorner1 = make_pair(getNodeData(b->boardList[b->saveCorner1.first], b->saveCorner1.second).x,
										getNodeData(b->boardList[b->saveCorner1.first], b->saveCorner1.second).y);
									pair <int, int> blockCorner2 = make_pair(getNodeData(b->boardList[b->saveCorner2.first], b->saveCorner2.second).x,
										getNodeData(b->boardList[b->saveCorner2.first], b->saveCorner2.second).y);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_U(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								System::playSound(3);
								Cell nullCell;
								nullCell.animal = 0;
								nullCell.state = DELETED;
								Cell currCell0 = getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second);
								if (currCell0.state == SELECTED) {
									currCell0.state = NORMAL;
									editNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second, currCell0);
								}
								Cell currCell1 = getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second);
								if (currCell1.state == SELECTED) {
									currCell1.state = NORMAL;
									editNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second, currCell1);
								}
								if (CurrSelectedCell[0].second < CurrSelectedCell[1].second) {
									RemovePos(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second);
									AddHead(b->boardList[CurrSelectedCell[0].first], nullCell);
									RemovePos(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second);
									AddHead(b->boardList[CurrSelectedCell[1].first], nullCell);
								}
								else {
									RemovePos(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second);
									AddHead(b->boardList[CurrSelectedCell[1].first], nullCell);
									RemovePos(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second);
									AddHead(b->boardList[CurrSelectedCell[0].first], nullCell);
								}
								bool over = 1;
								for (int i = 1; i <= 6; i++) {
									Node* p = b->boardList[i];
									while (p->pNext) {
										if (p->data.state != DELETED)
											over = 0;
										p = p->pNext;
									}
								}
								if (over) {
									drawWinning();
									System::playSound(6);
									overOut = 1;
									Sleep(10000);
									bool ask = drawAskBoard();
									if (ask) {
										vector <int> tmp = generateAnimal();
										shuffleBoard(b, tmp);
										currCell = { 1, 1 };
										currSuggested.first = 0;
										Sleep(2000);
									}
									else {
										currState = State[10];
									}
								}
							}
							else {
								System::playSound(4);
								player1.playerScore -= 5;
								Cell cell0 = getNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second);
								Cell cell1 = getNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second);
								cell0.state = cell1.state = NORMAL;
								editNodeData(b->boardList[CurrSelectedCell[0].first], CurrSelectedCell[0].second, cell0);
								editNodeData(b->boardList[CurrSelectedCell[1].first], CurrSelectedCell[1].second, cell1);
							}
							CurrSelectedCell.clear();
						}
					}
					else if (cell.state == SELECTED) {
						cell.state = NORMAL;
						editNodeData(b->boardList[currCell.first], currCell.second, cell);
						CurrSelectedCell.clear();
					}
				}
				else if (k == 11) {
					player1.playerScore -= 2;
					System::playSound(2);
					bool found = 0;
					for (int i1 = 1; i1 <= 6; i1++) {
						for (int j1 = 1; j1 <= 8; j1++) {
							for (int i2 = 1; i2 <= 6; i2++) {
								for (int j2 = 1; j2 <= 8; j2++) {
									if (!found && make_pair(i1, j1) != make_pair(i2, j2) &&
										getNodeData(b->boardList[i1], j1).animal != 0 && getNodeData(b->boardList[i2], j2).animal != 0 &&
										getNodeData(b->boardList[i1], j1).animal ==
										getNodeData(b->boardList[i2], j2).animal) {
										vector <pair <int, int>> currTest;
										currTest.clear();
										currTest.push_back(make_pair(i1, j1));
										currTest.push_back(make_pair(i2, j2));
										if (b->CheckLineIList(make_pair(i1, j1), make_pair(i2, j2)) ||
											b->CheckLineLList(make_pair(i1, j1), make_pair(i2, j2)) ||
											b->CheckLineZList(make_pair(i1, j1), make_pair(i2, j2)) ||
											b->CheckLineUList(make_pair(i1, j1), make_pair(i2, j2))) {
											found = 1;
											currSuggested = { found, {make_pair(i1, j1), make_pair(i2, j2)} };
										}
									}
								}
							}
						}
					}
				}
				else if (k == 1) {
					bool already = 0;
					for (auto& player : players1) {
						if (player.playerID == player1.playerID) {
							already = 1;
							player = player1;
							break;
						}
					}
					if (!already) {
						players1.push_back(player1);
						sort(players1.begin(), players1.end(), cmpPlayer);
					}
					currState = State[10];
				}
			}
			if (!overOut) {
				int nowTime = (clock() - startTime) / CLOCKS_PER_SEC;
				if (nowTime - currTime == 1) {
					currTime = nowTime;
					player1.playerTime++;
				}
			}
		}

		// Level 3
		// No LinkedList
		if (currState == gameMode[2]) {
			System::clearScreen();
			drawEscBoard();
			drawScoreBoard(player1);
			System::drawRectangle2(70, 10, 10, 3);
			System::gotoXY(71, 11);
			drawTimeBoard(player1);
			b->createBoard(currCell, currSuggested, Game::gameBG);
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					player1.playerBoard[i][j] = b->board[i][j];
			modifyPlayerInfoInFile(player1.playerID, player1, SAVE_FILE);
			bool already = 0;
			for (auto& player : players) {
				if (player.playerID == player1.playerID) {
					already = 1;
					player = player1;
					break;
				}
			}
			if (!already) {
				players.push_back(player1);
				sort(players.begin(), players.end(), cmpPlayer);
			}
			bool check = 0;
			for (int i1 = 1; i1 <= 6; i1++) {
				for (int j1 = 1; j1 <= 8; j1++) {
					for (int i2 = 1; i2 <= 6; i2++) {
						for (int j2 = 1; j2 <= 8; j2++) {
							if (!check && make_pair(i1, j1) != make_pair(i2, j2) &&
								b->getAnimal(make_pair(i1, j1)) != 0 && b->getAnimal(make_pair(i2, j2)) != 0 &&
								b->getAnimal(make_pair(i1, j1)) == b->getAnimal(make_pair(i2, j2))) {
								vector <pair <int, int>> currTest;
								currTest.clear();
								currTest.push_back(make_pair(i1, j1));
								currTest.push_back(make_pair(i2, j2));
								System::gotoXY(0, 0);
								if (b->CheckLineI(currTest[0], currTest[1]) ||
									(b->CheckLineL(currTest[0], currTest[1])) ||
									b->CheckLineZ(currTest[0], currTest[1]) ||
									b->CheckLineU(currTest[0], currTest[1])) {
									check = 1;
								}
							}
						}
					}
				}
			}
			if (!check) {
				System::gotoXY(50, 18);
				cout << "No moves left!";
				resetBoard(b);
			}
			bool overOut = 0;
			if (_kbhit()) {
				int k = System::getKeyboard();
				if (k == 2 || k == 6) {
					System::playSound(5);
					currCell.first--;
					if (currCell.first < 1)
						currCell.first += 6;
				}
				else if (k == 3 || k == 7) {
					System::playSound(5);
					currCell.second--;
					if (currCell.second < 1)
						currCell.second += 8;
				}
				else if (k == 4 || k == 8) {
					System::playSound(5);
					currCell.second++;
					if (currCell.second > 8)
						currCell.second -= 8;
				}
				else if (k == 5 || k == 9) {
					System::playSound(5);
					currCell.first++;
					if (currCell.first > 6)
						currCell.first -= 6;
				}
				else if (k == 10) {
					System::playSound(2);
					currSuggested.first = 0;
					if (b->board[currCell.first][currCell.second].state == NORMAL) {
						b->board[currCell.first][currCell.second].state = SELECTED;
						CurrSelectedCell.push_back(currCell);
						if (CurrSelectedCell.size() == 2) {
							if ((b->getAnimal(CurrSelectedCell[0]) == b->getAnimal(CurrSelectedCell[1])) &&
								(b->CheckLineI(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									(b->CheckLineL(CurrSelectedCell[0], CurrSelectedCell[1])) ||
									b->CheckLineZ(CurrSelectedCell[0], CurrSelectedCell[1]) ||
									b->CheckLineU(CurrSelectedCell[0], CurrSelectedCell[1]))) {
								player1.playerScore += 10;
								if (b->CheckLineI(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									b->DrawLine_I(block0, block1);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineL(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockAngle = b->getCoord(b->saveAngle);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockAngle.first += 2;
									blockAngle.second += 1;
									b->DrawLine_L(block0, block1, blockAngle);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineZ(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockCorner1 = b->getCoord(b->saveCorner1);
									pair <int, int> blockCorner2 = b->getCoord(b->saveCorner2);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_Z(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								else if (b->CheckLineU(CurrSelectedCell[0], CurrSelectedCell[1])) {
									pair <int, int> block0 = b->getCoord(CurrSelectedCell[0]);
									pair <int, int> block1 = b->getCoord(CurrSelectedCell[1]);
									pair <int, int> blockCorner1 = b->getCoord(b->saveCorner1);
									pair <int, int> blockCorner2 = b->getCoord(b->saveCorner2);
									block0.first += 2;
									block0.second += 1;
									block1.first += 2;
									block1.second += 1;
									blockCorner1.first += 2;
									blockCorner1.second += 1;
									blockCorner2.first += 2;
									blockCorner2.second += 1;
									b->DrawLine_U(block0, block1, blockCorner1, blockCorner2);
									Sleep(250);
									System::setBGandText(BRIGHT_WHITE, BLACK);
								}
								System::playSound(3);
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].state = DELETED;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].state = DELETED;
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].animal = 0;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].animal = 0;
								if (CurrSelectedCell[0].second < CurrSelectedCell[1].second)
									swap(CurrSelectedCell[0], CurrSelectedCell[1]);
								for (int j = CurrSelectedCell[0].second; j < 8; j++) {
									swap(b->board[CurrSelectedCell[0].first][j].animal, b->board[CurrSelectedCell[0].first][j + 1].animal);
									swap(b->board[CurrSelectedCell[0].first][j].state, b->board[CurrSelectedCell[0].first][j + 1].state);
								}
								for (int j = CurrSelectedCell[1].second; j < 8; j++) {
									swap(b->board[CurrSelectedCell[1].first][j].animal, b->board[CurrSelectedCell[1].first][j + 1].animal);
									swap(b->board[CurrSelectedCell[1].first][j].state, b->board[CurrSelectedCell[1].first][j + 1].state);
								}
								bool over = 1;
								for (int i = 1; i <= 6; i++)
									for (int j = 1; j <= 8; j++)
										if (b->getAnimal(make_pair(i, j)) != 0)
											over = 0;
								if (over) {
									drawWinning();
									System::playSound(6);
									overOut = 1;
									Sleep(10000);
									bool ask = drawAskBoard();
									if (ask) {
										vector <int> tmp = generateAnimal();
										shuffleBoard(b, tmp);
										currCell = { 1, 1 };
										currSuggested.first = 0;
										Sleep(2000);
									}
									else {
										currState = State[10];
									}
								}
							}
							else {
								System::playSound(4);
								player1.playerScore -= 5;
								b->board[CurrSelectedCell[0].first][CurrSelectedCell[0].second].state = NORMAL;
								b->board[CurrSelectedCell[1].first][CurrSelectedCell[1].second].state = NORMAL;
							}
							CurrSelectedCell.clear();
						}
					}
					else if (b->board[currCell.first][currCell.second].state == SELECTED) {
						b->board[currCell.first][currCell.second].state = NORMAL;
						CurrSelectedCell.clear();
					}
				}
				else if (k == 11) {
					bool found = 0;
					player1.playerScore -= 2;
					System::playSound(2);
					for (int i1 = 1; i1 <= 6; i1++) {
						for (int j1 = 1; j1 <= 8; j1++) {
							for (int i2 = 1; i2 <= 6; i2++) {
								for (int j2 = 1; j2 <= 8; j2++) {
									if (!found && make_pair(i1, j1) != make_pair(i2, j2) &&
										b->getAnimal(make_pair(i1, j1)) != 0 && b->getAnimal(make_pair(i2, j2)) != 0 &&
										b->getAnimal(make_pair(i1, j1)) == b->getAnimal(make_pair(i2, j2))) {
										vector <pair <int, int>> currTest;
										currTest.clear();
										currTest.push_back(make_pair(i1, j1));
										currTest.push_back(make_pair(i2, j2));
										if (b->CheckLineI(currTest[0], currTest[1]) ||
											(b->CheckLineL(currTest[0], currTest[1])) ||
											b->CheckLineZ(currTest[0], currTest[1]) ||
											b->CheckLineU(currTest[0], currTest[1])) {
											found = 1;
											currSuggested = { found, {make_pair(i1, j1), make_pair(i2, j2)} };
										}
									}
								}
							}
						}
					}
				}
				else if (k == 1) {
					bool already = 0;
					for (auto& player : players) {
						if (player.playerID == player1.playerID) {
							already = 1;
							player = player1;
							break;
						}
					}
					if (!already) {
						players.push_back(player1);
						sort(players.begin(), players.end(), cmpPlayer);
					}
					currState = State[10];
				}
			}
			if (!overOut) {
				int nowTime = (clock() - startTime) / CLOCKS_PER_SEC;
				if (nowTime - currTime == 1) {
					currTime = nowTime;
					player1.playerTime++;
				}
			}
		}
		if (currState == State[3])
			playing = false;
	}
	delete b;
}
