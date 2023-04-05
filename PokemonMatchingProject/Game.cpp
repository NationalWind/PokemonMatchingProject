#include "Game.h"


string Game::State[11] = { "Play", "Settings", "Leaderboards", "Instructions", "Exit", "Back",
								"1 Player", "2 Players", "Easy", "Hard", "Main Menu"};

string Game::gameMode[4] = { "1 Player Easy", "1 Player Hard", "2 Players Easy", "2 Players Hard" };

string Game::getProfile[2] = {"Profile 1", "Profile 2"};

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


vector <int> generateAnimal() {
	vector <int> tmp; // save the characters for shuffling
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
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
			if (i == 0 || i == 7 || j == 0 || j == 9) {
				b->board[i][j].animal = 0;
				b->board[i][j].state = DELETED;
			}
			else {
				b->board[i][j].animal = 0;
				b->board[i][j].state = NORMAL;
			}
	int row = 1, col = 1;
	for (int i = 0; i < tmp.size(); i++)
	{
		b->board[row][col].animal = tmp[i];
		col++;
		if (col > 8) {
			row++;
			col = 1;
		}
	}
}

void resetBoard(Board* &b) {
	vector <int> tmp;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
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
}

void drawEscBoard() {
	System::drawRectangle(0, 0, 40, 3);
	System::gotoXY(1, 1);
	cout << "Press [Esc] to turn back to Main Menu...";
}

void drawScoreBoard(int score){
	System::drawRectangle(50, 20, 22, 5);
	System::gotoXY(51, 21);
	cout << "Score: " << score;
	System::gotoXY(51, 23);
	cout << "[H]: Move Suggestion";
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
	Menu::printCenterText("Do you want to play this mode again?", 12);
	bool currAsk = 1;
	int k = 0;
	do {
		System::gotoXY(57, 13);
		if (currAsk)
			System::setBGandText(YELLOW, BLUE);
		cout << "YES";
		System::gotoXY(61, 13);
		if (!currAsk)
			System::setBGandText(BRIGHT_WHITE, BLACK);
		cout << "NO";
		k = System::getKeyboard();
		if (k == 3 || k == 4 || k == 7 || k == 8)
			currAsk = !currAsk;
	} while (k != 10);
	System::setBGandText(BRIGHT_WHITE, BLACK);
	return currAsk;
}

bool cmpPlayer(Player a, Player b) {
	return a.playerScore > b.playerScore;
}

void Game::inGame() {
	srand(time(nullptr));
	string currState = State[10];
	bool playing = true;
	bool newPlayer = 0;
	int currMenuSelection = 0; // main menu counter
	int currPlayerSelection = 0; // player menu counter
	int currGameSelection = 0; // game menu counter
	pair <int, int> currCell = { 1, 1 };
	pair <bool, pair <pair <int, int>, pair <int, int>>> currSuggested = { 0, {{0, 0}, {0, 0}} };
	int gameChoosing = 0;
	vector <Player> players;
	players = readAllPlayerInfoFromFile(SAVE_FILE);
	sort(players.begin(), players.end(), cmpPlayer);
	Board *b = new Board(8, 10, 5, 5);
	// Cell: 3x5
	// Board: 6x8 --> real: 8x10
	vector<pair<int, int>>CurrSelectedCell;
	Player player1;
	Player player2;
	Player tmpPlayer;
	System::playSound(0);
	while (playing) {
		//mainMenu
		if (currState == State[10]) {
			Menu::mainMenu();
			Menu::menuInOrder(State, currMenuSelection, 20, 4);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currMenuSelection++;
				if (currMenuSelection > 4)
					currMenuSelection -= 5;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currMenuSelection--;
				if (currMenuSelection < 0)
					currMenuSelection += 5;
			}
			if (k == 10) {
				System::playSound(2);
				currState = State[currMenuSelection];
			}
		}
		// choose number of players menu
		if (currState == State[0]) {
			Menu::playerModes();
			string menuPlayer[3] = { State[6], State[7], State[5] };
			Menu::menuInOrder(menuPlayer, currPlayerSelection, 20, 2);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currPlayerSelection++;
				if (currPlayerSelection > 2)
					currPlayerSelection -= 3;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currPlayerSelection--;
				if (currPlayerSelection < 0)
					currPlayerSelection += 3;
			}
			if (k == 10) {
				System::playSound(2);
				if (currPlayerSelection == 2)
					currState = State[10];
				else
					currState = menuPlayer[currPlayerSelection];
			}
		}
		// Settings
		if (currState == State[1]) {
			System::clearScreen();
			drawEscBoard();
			int k = System::getKeyboard();
			if (k == 1) {
				System::playSound(2);
				currState = State[10];
			}
		}
		// Leaderboards
		if (currState == State[2]) {
			System::clearScreen();
			drawEscBoard();
			Menu::leaderboardsMenu();
			System::gotoXY(22, 12);
			cout << "No.";
			System::gotoXY(30, 12);
			cout << "Player";
			System::gotoXY(80, 12);
			cout << "Score";
			int playerY = 15, cnt = 0;
			for (auto &player : players) {
				System::gotoXY(22, playerY);
				cout << ++cnt << ".";
				System::gotoXY(30, playerY);
				cout << player.playerName;
				System::gotoXY(80, playerY);
				cout << player.playerScore;
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
		// 1 Player game mod choosing
		if (currState == State[6]) {
			Menu::gameModes();
			string menuPlayer[3] = { State[8], State[9], State[5] };
			Menu::menuInOrder(menuPlayer, currGameSelection, 20, 2);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currGameSelection++;
				if (currGameSelection > 2)
					currGameSelection -= 3;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currGameSelection--;
				if (currGameSelection < 0)
					currGameSelection += 3;
			}
			if (k == 10)
			{
				System::playSound(2);
				if (currGameSelection == 2)
					currState = State[0];
				else {
					if (currGameSelection == 0) {
						gameChoosing = 0;
						player1.playerMode = 0;
					}
					else {
						gameChoosing = 1;
						player1.playerMode = 1;
					}
					currState = getProfile[0];
				}
			}
		}

		//2 players game mode choosing
		if (currState == State[7]) {
			Menu::gameModes();
			string menuPlayer[3] = { State[8], State[9], State[5] };
			Menu::menuInOrder(menuPlayer, currGameSelection, 20, 2);
			int k = System::getKeyboard();
			if (k == 5 || k == 9) {
				System::playSound(1);
				currGameSelection++;
				if (currGameSelection > 2)
					currGameSelection -= 3;
			}
			if (k == 2 || k == 6) {
				System::playSound(1);
				currGameSelection--;
				if (currGameSelection < 0)
					currGameSelection += 3;
			}
			if (k == 10)
			{
				System::playSound(2);
				if (currGameSelection == 2)
					currState = State[0];
				else {
					if (currGameSelection == 0) {
						gameChoosing = 2;
						player1.playerMode = 0;
						player2.playerMode = 0;
					}
					else {
						gameChoosing = 3;
						player1.playerMode = 1;
						player2.playerMode = 1;
					}
					currState = getProfile[1];
				}
			}
		}
		//info 1 player
		if (currState == getProfile[0]) {
			bool registered = 0;
			do {
				System::clearScreen();
				System::drawRectangle2(18, 14, 25, 10);
				Menu::printCenterText("Player Profile", 10);
				System::gotoXY(20, 15);
				cout << "Username: ";
				cin >> player1.playerName;
				for (auto &player : players) {
					if (strcmp(player.playerName, player1.playerName) == 0) {
						tmpPlayer = player;
						registered = 1;
						break;
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
						newPlayer = 1;
						player1 = tmpPlayer;
						System::gotoXY(20, 22);
						System::playSound(2);
						cout << "Welcome Back <3";
						Sleep(500);
					}
				}
				else {
					System::gotoXY(20, 22);
					player1.playerID = System::randBetween(10000, 99999);
					savePlayerInfoToFile(player1, SAVE_FILE);
					cout << "Welcome New Player :3";
					System::playSound(2);
					Sleep(500);
					registered = 1;
					for (int i = 1; i <= 6; i++)
						for (int j = 1; j <= 8; j++)
							b->board[i][j].state = NORMAL;
					player1.playerScore = 0;
					vector <int> tmp;
					tmp = generateAnimal();
					shuffleBoard(b, tmp);
				}
			} while (!registered);
			
			currState = gameMode[gameChoosing];
		}
		//info 2 players
		if (currState == getProfile[1]) {
			System::clearScreen();
			Menu::printCenterText("Player Profile", 10);
			System::gotoXY(5, 13);
			cout << "Player 1";
			System::gotoXY(65, 13);
			cout << "Player 2";
			player1.inputProfile(5);
			player2.inputProfile(65);
			savePlayerInfoToFile(player1, SAVE_FILE2);
			savePlayerInfoToFile(player2, SAVE_FILE2);
			currState = gameMode[gameChoosing];
		}
		// play the game
		if (currState == gameMode[0]) {
			System::clearScreen();
			drawEscBoard();
			drawScoreBoard(player1.playerScore);
			b->createBoard(currCell, currSuggested);
			for (int i = 0; i < 8; i++)
				for (int j = 0; j < 10; j++)
					player1.playerBoard[i][j] = b->board[i][j];
			modifyPlayerInfoInFile(player1.playerID, player1, SAVE_FILE);
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
								Sleep(10000);
								bool ask = drawAskBoard();
								if (ask) {
									vector <int> tmp = generateAnimal();
									shuffleBoard(b, tmp);
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

				modifyPlayerInfoInFile(player1.playerID, player1, SAVE_FILE);
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
									System::gotoXY(0, 0);
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
				currState = State[10];
			}
		}
		if (currState == State[4])
			playing = false;
	}
	delete b;
}
