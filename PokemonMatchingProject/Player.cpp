#include "Player.h"

Player::Player() {
	memset(playerName, 0, sizeof(playerName));
	memset(playerPassword, 0, sizeof(playerPassword));
	playerScore = 0;
	playerMode = 0;
	playerTime = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
			playerBoard[i][j] = Cell();
}

void Player::inputProfile(int x) {
	System::gotoXY(x, 15);
	cout << "Username: ";
	cin >> playerName;
	System::gotoXY(x, 18);
	cout << "Password: ";
	cin >> playerPassword;
}