#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <utility>
#include "Cell.h"
#include "System.h"


using namespace std;

// linked list node
struct Node {
	Cell data;
	Node* pNext;
	Node() : data(), pNext(nullptr) {};
};

Node* CreateNode(Cell);
void AddHead(Node*&, Cell);
void AddTail(Node*&, Cell);
void RemoveHead(Node*&);
void RemovePos(Node*&, int);
void editNodeData(Node*&, int, Cell);
Cell getNodeData(Node*& head, int nodeIndex);


struct Board {
	// board size
	int height;
	int width;

	// the board's topleft corner coordinate
	int boardX;
	int boardY;

	// main board
	Cell** board;
	// main board but by linked lists
	Node* boardList[8];

	//save the corners for drawing I, U, Z, L
	pair <int, int> saveAngle;
	pair <int, int> saveCorner1;
	pair <int, int> saveCorner2;

	Board(int, int, int, int);
	Board();
	~Board();

	//get info from board
	int getAnimal(pair<int, int>);
	int getState(pair<int, int>);
	pair <int, int> getCoord(pair<int, int>);

	// draw Lines
	void DrawLine_I(pair<int, int>, pair<int, int>);
	void DrawLine_L(pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_Z(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);
	void DrawLine_U(pair<int, int>, pair<int, int>, pair<int, int>, pair<int, int>);

	// draw board
	void createBoard(pair <int, int>, pair <bool, pair <pair <int, int>, pair <int, int>>>, string[]);
	void createBoardList(pair <int, int>, pair <bool, pair <pair <int, int>, pair <int, int>>>, string[]);

	// check lines
	bool CheckLineI(pair<int, int>, pair<int, int>);
	bool CheckLineL(pair<int, int>, pair<int, int>);
	bool CheckLineZ(pair<int, int>, pair<int, int>);
	bool CheckLineU(pair<int, int>, pair<int, int>);
	// check lines List

	bool CheckLineIList(pair<int, int>, pair<int, int>);
	bool CheckLineLList(pair<int, int>, pair<int, int>);
	bool CheckLineZList(pair<int, int>, pair<int, int>);
	bool CheckLineUList(pair<int, int>, pair<int, int>);
};