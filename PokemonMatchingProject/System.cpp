<<<<<<< HEAD
#include "System.h"

HWND System::consoleWindow = GetConsoleWindow();
HANDLE System::hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE System::hInput = GetStdHandle(STD_INPUT_HANDLE);

void System::setScreenResolution() {
	RECT rectClient, rectWindow;
	GetClientRect(consoleWindow, &rectClient), GetWindowRect(consoleWindow, &rectWindow);
}

void System::disableMouseInput()
{
	DWORD prev_mode;
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

void System::clearScreen() {
	Sleep(50);
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	cout.flush();
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

void System::setupScreen() {
	setScreenResolution();
	disableMouseInput();
	setFont();
	hideScrollBars();
	disableFullscreen();
	setCursor(false);
}

void System::setFont() {
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(info);
	GetCurrentConsoleFontEx(hOutput, FALSE, &info);
	info.dwFontSize.X = 12;
	info.dwFontSize.Y = 24;
	wcscpy_s(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hOutput, FALSE, &info);
}

void System::hideScrollBars() {
	ShowScrollBar(consoleWindow, SB_BOTH, 0);
}

void System::setBGandText(int background, int text) {
	SetConsoleTextAttribute(hOutput, background * 16 + text);
}

void System::disableFullscreen() {
	SetWindowLong(consoleWindow, GWL_STYLE,
		GetWindowLong(consoleWindow, GWL_STYLE) & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME));
}

void System::setCursor(bool show) {
	CONSOLE_CURSOR_INFO info = { 1, show };
	SetConsoleCursorInfo(hOutput, &info);
}

void System::gotoXY(int x, int y) {
	SetConsoleCursorPosition(hOutput, COORD{ (short)x, (short)y });
}

COORD System::getXY()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(System::hOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void System::setKeyboardInputEnabled(bool enabled)
{
	DWORD originalMode = 0;
	GetConsoleMode(System::hInput, &originalMode);

	if (enabled) {
		SetConsoleMode(System::hInput, originalMode | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
	}
	else {
		SetConsoleMode(System::hInput, originalMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT));
	}
}

int System::getKeyboard() {
	int c = _getch();
	// Arrow key
	if (c == 0 || c == 224) {
		switch (_getch()) {
		case KEY_UP:
			return 2;
		case KEY_LEFT:
			return 3;
		case KEY_RIGHT:
			return 4;
		case KEY_DOWN:
			return 5;
		default:
			return 0;
		}
	}
	else {
		if (c == KEY_ESC)
			return 1;
		else if (c == KEY_W || c == KEY_w)
			return 6;
		else if (c == KEY_A || c == KEY_a)
			return 7;
		else if (c == KEY_D || c == KEY_d)
			return 8;
		else if (c == KEY_S || c == KEY_s)
			return 9;
		else if (c == KEY_ENTER)
			return 10;
		else if (c == 72 || c == 104) //h, h
			return 11;
		//else if (c == 77 || c == 109) // m, m
		//	return 8;
		else
			return 0;
	}
}

void System::exitConsole() {
	exit(0);
}

void System::drawRectangle(int x, int y, int width, int height) {
	// Define the Unicode characters
	char topLeft = 218;
	char topRight = 191;
	char bottomLeft = 192;
	char bottomRight = 217;
	char horizontal = 196;
	char vertical = 179;

	System::gotoXY(x, y);
	cout << topLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << topRight << endl;

	for (int i = 1; i < height - 1; i++) {
		System::gotoXY(x, y + i);
		cout << vertical;
		System::gotoXY(x + width - 1, y + i);
		cout << vertical;
		cout << endl;
	}

	System::gotoXY(x, y + height - 1);
	cout << bottomLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << bottomRight << endl;
}



void System::drawRectangle2(int x, int y, int width, int height) {
	char topLeft = 201;
	char topRight = 187;
	char bottomLeft = 200;
	char bottomRight = 188;
	char horizontal = 205;
	char vertical = 186;

	System::gotoXY(x, y);
	cout << topLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << topRight << endl;

	for (int i = 1; i < height - 1; i++) {
		System::gotoXY(x, y + i);
		cout << vertical;
		System::gotoXY(x + width - 1, y + i);
		cout << vertical;
		cout << endl;
	}

	System::gotoXY(x, y + height - 1);
	cout << bottomLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << bottomRight << endl;
}

int System::randBetween(int a, int b) {
	int randNum = rand() % (b - a + 1) + a;
	return randNum;
}

void System::playSound(int i)
{
	static vector<const wchar_t*> soundFile{ L"background.wav", L"selecting.wav", L"choose.wav", L"correct.wav", L"incorrect.wav", L"move.wav",  L"winning.wav"};
	PlaySound(soundFile[i], NULL, SND_FILENAME | SND_ASYNC);
}

void System::stopSound()
{
	PlaySound(NULL, NULL, SND_PURGE);
=======
#include "System.h"

HWND System::consoleWindow = GetConsoleWindow();
HANDLE System::hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE System::hInput = GetStdHandle(STD_INPUT_HANDLE);

void System::setScreenResolution() {
	RECT rectClient, rectWindow;
	GetClientRect(consoleWindow, &rectClient), GetWindowRect(consoleWindow, &rectWindow);
}

void System::disableMouseInput()
{
	DWORD prev_mode;
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
}

void System::clearScreen() {
	Sleep(50);
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	cout.flush();
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

void System::setupScreen() {
	setScreenResolution();
	disableMouseInput();
	setFont();
	hideScrollBars();
	disableFullscreen();
	setCursor(false);
}

void System::setFont() {
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(info);
	GetCurrentConsoleFontEx(hOutput, FALSE, &info);
	info.dwFontSize.X = 12;
	info.dwFontSize.Y = 24;
	wcscpy_s(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hOutput, FALSE, &info);
}

void System::hideScrollBars() {
	ShowScrollBar(consoleWindow, SB_BOTH, 0);
}

void System::setBGandText(int background, int text) {
	SetConsoleTextAttribute(hOutput, background * 16 + text);
}

void System::disableFullscreen() {
	SetWindowLong(consoleWindow, GWL_STYLE,
		GetWindowLong(consoleWindow, GWL_STYLE) & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME));
}

void System::setCursor(bool show) {
	CONSOLE_CURSOR_INFO info = { 1, show };
	SetConsoleCursorInfo(hOutput, &info);
}

void System::gotoXY(int x, int y) {
	SetConsoleCursorPosition(hOutput, COORD{ (short)x, (short)y });
}

COORD System::getXY()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(System::hOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	}
	else {
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

void System::setKeyboardInputEnabled(bool enabled)
{
	DWORD originalMode = 0;
	GetConsoleMode(System::hInput, &originalMode);

	if (enabled) {
		SetConsoleMode(System::hInput, originalMode | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
	}
	else {
		SetConsoleMode(System::hInput, originalMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT));
	}
}

int System::getKeyboard() {
	int c = _getch();
	// Arrow key
	if (c == 0 || c == 224) {
		switch (_getch()) {
		case KEY_UP:
			return 2;
		case KEY_LEFT:
			return 3;
		case KEY_RIGHT:
			return 4;
		case KEY_DOWN:
			return 5;
		default:
			return 0;
		}
	}
	else {
		if (c == KEY_ESC)
			return 1;
		else if (c == KEY_W || c == KEY_w)
			return 6;
		else if (c == KEY_A || c == KEY_a)
			return 7;
		else if (c == KEY_D || c == KEY_d)
			return 8;
		else if (c == KEY_S || c == KEY_s)
			return 9;
		else if (c == KEY_ENTER)
			return 10;
		else if (c == 72 || c == 104) //h, h
			return 11;
		//else if (c == 77 || c == 109) // m, m
		//	return 8;
		else
			return 0;
	}
}

void System::exitConsole() {
	exit(0);
}

void System::drawRectangle(int x, int y, int width, int height) {
	// Define the Unicode characters
	char topLeft = 218;
	char topRight = 191;
	char bottomLeft = 192;
	char bottomRight = 217;
	char horizontal = 196;
	char vertical = 179;

	System::gotoXY(x, y);
	cout << topLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << topRight << endl;

	for (int i = 1; i < height - 1; i++) {
		System::gotoXY(x, y + i);
		cout << vertical;
		System::gotoXY(x + width - 1, y + i);
		cout << vertical;
		cout << endl;
	}

	System::gotoXY(x, y + height - 1);
	cout << bottomLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << bottomRight << endl;
}



void System::drawRectangle2(int x, int y, int width, int height) {
	char topLeft = 201;
	char topRight = 187;
	char bottomLeft = 200;
	char bottomRight = 188;
	char horizontal = 205;
	char vertical = 186;

	System::gotoXY(x, y);
	cout << topLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << topRight << endl;

	for (int i = 1; i < height - 1; i++) {
		System::gotoXY(x, y + i);
		cout << vertical;
		System::gotoXY(x + width - 1, y + i);
		cout << vertical;
		cout << endl;
	}

	System::gotoXY(x, y + height - 1);
	cout << bottomLeft;

	for (int i = 1; i < width - 1; i++) {
		cout << horizontal;
	}
	cout << bottomRight << endl;
}

int System::randBetween(int a, int b) {
	int randNum = rand() % (b - a + 1) + a;
	return randNum;
}

void System::playSound(int i)
{
	static vector<const wchar_t*> soundFile{ L"background.wav", L"selecting.wav", L"choose.wav", L"correct.wav", L"incorrect.wav", L"move.wav",  L"winning.wav"};
	PlaySound(soundFile[i], NULL, SND_FILENAME | SND_ASYNC);
}

void System::stopSound()
{
	PlaySound(NULL, NULL, SND_PURGE);
>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
}