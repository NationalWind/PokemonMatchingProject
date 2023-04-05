<<<<<<< HEAD
#pragma once
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <random>

// colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

// key code
#define KEY_BACK 8
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_W 87
#define KEY_w 119
#define KEY_A 65
#define KEY_a 97
#define KEY_S 83
#define KEY_s 115
#define KEY_D 68
#define KEY_d 100

using namespace std;

struct System {
	static HWND consoleWindow; // pointer for console window handle
	static HANDLE hOutput; //pointer for console screen output
	static HANDLE hInput; //pointer for console screen input
	
	static void clearScreen();
	static void setScreenResolution(); 
	static void setupScreen();
	static void disableMouseInput();
	static void setFont();
	static void hideScrollBars();
	static void setBGandText(int, int); // set color for background and text
	static void disableFullscreen(); // Disallow fullscreen
	static void setCursor(bool); // Show/Hide cursor
	static void gotoXY(int, int); // move to XY
	static COORD getXY(); // get XY
	static void setKeyboardInputEnabled(bool);
	static int getKeyboard(); // get value from keyboard
	static void exitConsole();
	static void drawRectangle(int, int, int, int); // 1 edge
	static void drawRectangle2(int, int, int, int); // 2 edge
	static int randBetween(int, int);
	static void playSound(int);
	static void stopSound();
};
=======
#pragma once
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <random>

// colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

// key code
#define KEY_BACK 8
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESC 27
#define KEY_ENTER 13
#define KEY_W 87
#define KEY_w 119
#define KEY_A 65
#define KEY_a 97
#define KEY_S 83
#define KEY_s 115
#define KEY_D 68
#define KEY_d 100

using namespace std;

struct System {
	static HWND consoleWindow; // pointer for console window handle
	static HANDLE hOutput; //pointer for console screen output
	static HANDLE hInput; //pointer for console screen input
	
	static void clearScreen();
	static void setScreenResolution(); 
	static void setupScreen();
	static void disableMouseInput();
	static void setFont();
	static void hideScrollBars();
	static void setBGandText(int, int); // set color for background and text
	static void disableFullscreen(); // Disallow fullscreen
	static void setCursor(bool); // Show/Hide cursor
	static void gotoXY(int, int); // move to XY
	static COORD getXY(); // get XY
	static void setKeyboardInputEnabled(bool);
	static int getKeyboard(); // get value from keyboard
	static void exitConsole();
	static void drawRectangle(int, int, int, int); // 1 edge
	static void drawRectangle2(int, int, int, int); // 2 edge
	static int randBetween(int, int);
	static void playSound(int);
	static void stopSound();
};
>>>>>>> 81853d676cc658c5e3f303c87e74263d4bde0ff8
