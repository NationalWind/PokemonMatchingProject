#include "System.h"
#include "Game.h"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); cout.tie(NULL);
	
	System::setupScreen();
	Game::inGame();
	return 0;
}
