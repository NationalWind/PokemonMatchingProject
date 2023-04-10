#include "Board.h"

Node* CreateNode(Cell data) {
	Node* p = new Node;
	p->data = data;
	p->pNext = NULL;
	return p;
}

void AddHead(Node*& pHead, Cell data) {
	if (pHead == NULL)
		pHead = CreateNode(data);
	else {
		Node* p = CreateNode(data);
		p->pNext = pHead;
		pHead = p;
	}
}

void AddTail(Node*& pHead, Cell data) {
	if (pHead == NULL)
		pHead = CreateNode(data);
	else {
		Node* p = pHead;
		while (p->pNext) {
			p = p->pNext;
		}
		p->pNext = CreateNode(data);
	}
}

void RemoveHead(Node*& pHead) {
	if (pHead == NULL) return;
	Node* p = pHead;
	pHead = pHead->pNext;
	delete p;
}

void RemovePos(Node*& pHead, int pos) {
	if (pos == 0) {
		RemoveHead(pHead);
		return;
	}
	int count = 0;
	Node* p = pHead;
	while (p != NULL && count < pos - 1) {
		p = p->pNext;
		count++;
	}

	if (p == NULL || p->pNext == NULL) return;
	Node* p2 = p->pNext->pNext;
	Node* p1 = p->pNext;
	p->pNext = p2;
	delete p1;
}

void editNodeData(Node*& head, int nodeIndex, Cell newData) {
	Node* currentNode = head;
	int currentIndex = 0;
	while (currentNode != nullptr && currentIndex < nodeIndex) {
		currentNode = currentNode->pNext;
		currentIndex++;
	}
	if (currentNode != nullptr) {
		currentNode->data = newData;
	}
}

Cell getNodeData(Node*& head, int nodeIndex) {
	Node* currentNode = head;
	int currentIndex = 0;
	while (currentNode != nullptr && currentIndex < nodeIndex) {
		currentNode = currentNode->pNext;
		currentIndex++;
	}
	if (currentNode != nullptr) {
		return currentNode->data;
	}
	else
		return Cell();
}

Board::Board() : height(0), width(0), boardX(0), boardY(0), board(nullptr) {
	for (int i = 0; i < 8; i++)
		boardList[i] = nullptr;
}

Board::Board(int _height = 0, int _width = 0, int _boardX = 0, int _boardY = 0) {
	height = _height;
	width = _width;
	boardX = _boardX;
	boardY = _boardY;
	saveAngle = { 0, 0 };
	saveCorner1 = { 0, 0 };
	saveCorner2 = { 0, 0 };
	board = new Cell * [height];
	for (int i = 0; i < height; i++)
		board[i] = new Cell[width];
	for (int i = 0; i < 8; i++)
		boardList[i] = nullptr;
}

Board::~Board() {
	for (int i = 0; i < height; i++)
		delete[]board[i];
	delete[]board;
	for (int i = 0; i < height; i++)
		delete boardList[i];
}
int Board::getAnimal(pair <int, int> Block) {
	return board[Block.first][Block.second].animal;
}
int Board::getState(pair <int, int> Block) {
	return board[Block.first][Block.second].state;
}
pair<int, int> Board::getCoord(pair <int, int> Block) {
	return { board[Block.first][Block.second].x, board[Block.first][Block.second].y };
}

void Board::DrawLine_I(pair<int, int> Selected_Block_1, pair<int, int> Selected_Block_2) {
	System::setBGandText(BLUE, BRIGHT_WHITE);
	// vertical
	if (Selected_Block_1.first == Selected_Block_2.first) {
		if (Selected_Block_1.second > Selected_Block_2.second)
			swap(Selected_Block_1, Selected_Block_2);
		System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
		putchar(30);
		for (int i = Selected_Block_1.second + 2; i < Selected_Block_2.second - 1; i++) {
			System::gotoXY(Selected_Block_1.first, i);
			putchar(179);
		}
		System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
		putchar(31);
		return;
	}
	// horizontal
	if (Selected_Block_1.second == Selected_Block_2.second) {
		if (Selected_Block_1.first > Selected_Block_2.first)
			swap(Selected_Block_1, Selected_Block_2);
		System::gotoXY(Selected_Block_1.first + 1, Selected_Block_1.second);
		putchar(17);
		for (int i = Selected_Block_1.first + 2; i < Selected_Block_2.first - 1; i++) {
			System::gotoXY(i, Selected_Block_1.second);
			putchar(45);
		}
		System::gotoXY(Selected_Block_2.first - 1, Selected_Block_2.second);
		putchar(16);
		return;
	}
}
void Board::DrawLine_L(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2,
	pair <int, int> Angle) {
	if (Selected_Block_1.second > Selected_Block_2.second)
		swap(Selected_Block_1, Selected_Block_2);
	System::setBGandText(BLUE, BRIGHT_WHITE);
	if (Selected_Block_1.first < Selected_Block_2.first) {
		//1. up-right corner
		if ((Angle.first == Selected_Block_2.first) && (Angle.second == Selected_Block_1.second)) {
			System::gotoXY(Selected_Block_1.first + 1, Selected_Block_1.second);
			putchar(17);
			for (int i = Selected_Block_1.first + 2; i <= Angle.first; i++) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Angle.second + 1; i <= Selected_Block_2.second - 2; i++) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
			putchar(31);
			return;
		}
		//3. down-left corner
		if ((Angle.first == Selected_Block_1.first) && (Angle.second == Selected_Block_2.second)) {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
			putchar(30);
			for (int i = Selected_Block_1.second + 2; i <= Angle.second - 1; i++) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Angle.first; i <= Selected_Block_2.first - 2; i++) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first - 1, Selected_Block_2.second);
			putchar(16);
			return;
		}
	}
	else {
		//2. down-right corner
		if ((Angle.first == Selected_Block_1.first) && (Angle.second == Selected_Block_2.second)) {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
			putchar(30);
			for (int i = Selected_Block_1.second + 2; i <= Angle.second - 1; i++) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Angle.first; i >= Selected_Block_2.first + 2; i--) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first + 1, Selected_Block_2.second);
			putchar(17);
			return;
		}
		//4. up-left corner
		if ((Angle.first == Selected_Block_2.first) && (Angle.second == Selected_Block_1.second)) {
			System::gotoXY(Selected_Block_1.first - 1, Selected_Block_1.second);
			putchar(16);
			for (int i = Selected_Block_1.first - 2; i >= Angle.first; i--) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Angle.second + 1; i <= Selected_Block_2.second - 1; i++) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
			putchar(31);
			return;
		}
	}
}


void Board::DrawLine_Z(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2,
	pair<int, int>Corner1, pair<int, int>Corner2) {
	if (Selected_Block_1.second > Selected_Block_2.second)
		swap(Selected_Block_1, Selected_Block_2);
	System::setBGandText(BLUE, BRIGHT_WHITE);
	if (Selected_Block_1.first < Selected_Block_2.first) {
		// vertical from left to right
		if (Corner1.first == Corner2.first) {
			System::gotoXY(Selected_Block_1.first + 1, Selected_Block_1.second);
			putchar(17);
			for (int i = Selected_Block_1.first + 2; i <= Corner1.first; i++) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Corner1.second + 1; i <= Corner2.second; i++) {
				System::gotoXY(Corner1.first, i);
				putchar(179);
			}
			for (int i = Corner2.first; i <= Selected_Block_2.first - 2; i++) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first - 1, Selected_Block_2.second);
			putchar(16);
			return;
		}
		// horizontal from left to right
		if (Corner1.second == Corner2.second) {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
			putchar(30);
			for (int i = Selected_Block_1.second + 2; i <= Corner1.second - 1; i++) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Corner1.first; i <= Corner2.first; i++) {
				System::gotoXY(i, Corner1.second);
				putchar(45);
			}
			for (int i = Corner2.second + 1; i <= Selected_Block_2.second - 2; i++) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
			putchar(31);
			return;
		}
	}
	else {
		// vertical from right to left
		if (Corner1.first == Corner2.first) {
			System::gotoXY(Selected_Block_1.first - 1, Selected_Block_1.second);
			putchar(16);
			for (int i = Selected_Block_1.first - 2; i >= Corner1.first; i--) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Corner1.second + 1; i <= Corner2.second - 1; i++) {
				System::gotoXY(Corner1.first, i);
				putchar(179);
			}
			for (int i = Corner2.first; i >= Selected_Block_2.first + 2; i--) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first + 1, Selected_Block_2.second);
			putchar(17);
			return;
		}
		// horizontal from right to left
		if (Corner1.second == Corner2.second) {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
			putchar(30);
			for (int i = Selected_Block_1.second + 2; i <= Corner1.second - 1; i++) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Corner1.first; i >= Corner2.first; i--) {
				System::gotoXY(i, Corner1.second);
				putchar(45);
			}
			for (int i = Corner2.second + 1; i <= Selected_Block_2.second - 2; i++) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
			putchar(31);
			return;
		}
	}
}

void Board::DrawLine_U(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2,
	pair<int, int>Corner1, pair<int, int>Corner2) {
	System::setBGandText(BLUE, BRIGHT_WHITE);
	if (Corner1.second == Corner2.second) {
		if (Selected_Block_1.first > Selected_Block_2.first) {
			swap(Selected_Block_1, Selected_Block_2);
			swap(Corner1, Corner2);
		}
		//1. North
		if (Corner1.second < Selected_Block_1.second) {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second - 1);
			putchar(31);
			for (int i = Selected_Block_1.second - 2; i >= Corner1.second + 1; i--) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Corner1.first; i <= Corner2.first; i++) {
				System::gotoXY(i, Corner1.second);
				putchar(45);
			}
			for (int i = Corner2.second + 1; i <= Selected_Block_2.second - 2; i++) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second - 1);
			putchar(31);
		}
		//3. South
		else {
			System::gotoXY(Selected_Block_1.first, Selected_Block_1.second + 1);
			putchar(30);
			for (int i = Selected_Block_1.second + 2; i <= Corner1.second - 1; i++) {
				System::gotoXY(Selected_Block_1.first, i);
				putchar(179);
			}
			for (int i = Corner1.first; i <= Corner2.first; i++) {
				System::gotoXY(i, Corner1.second);
				putchar(45);
			}
			for (int i = Corner2.second - 1; i >= Selected_Block_2.second + 2; i--) {
				System::gotoXY(Selected_Block_2.first, i);
				putchar(179);
			}
			System::gotoXY(Selected_Block_2.first, Selected_Block_2.second + 1);
			putchar(30);
		}
	}
	else if (Corner1.first == Corner2.first){
		if (Selected_Block_1.second > Selected_Block_2.second) {
			swap(Selected_Block_1, Selected_Block_2);
			swap(Corner1, Corner2);
		}
		//2. East
		if (Corner1.first > Selected_Block_1.first) {
			System::gotoXY(Selected_Block_1.first + 1, Selected_Block_1.second);
			putchar(17);
			for (int i = Selected_Block_1.first + 2; i <= Corner1.first; i++) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Corner1.second + 1; i <= Corner2.second - 1; i++) {
				System::gotoXY(Corner1.first, i);
				putchar(179);
			}
			for (int i = Corner2.first; i >= Selected_Block_2.first + 2; i--) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first + 1, Selected_Block_2.second);
			putchar(17);
		}
		//4. West
		else {
			System::gotoXY(Selected_Block_1.first - 1, Selected_Block_1.second);
			putchar(16);
			for (int i = Selected_Block_1.first - 2; i >= Corner1.first; i--) {
				System::gotoXY(i, Selected_Block_1.second);
				putchar(45);
			}
			for (int i = Corner1.second + 1; i <= Corner2.second - 1; i++) {
				System::gotoXY(Corner1.first, i);
				putchar(179);
			}
			for (int i = Corner2.first; i <= Selected_Block_2.first - 2; i++) {
				System::gotoXY(i, Selected_Block_2.second);
				putchar(45);
			}
			System::gotoXY(Selected_Block_2.first - 1, Selected_Block_2.second);
			putchar(16);
		}
	}
}

void Board::createBoard(pair <int, int> currCell, pair <bool, pair <pair <int, int>, pair <int, int>>> currSuggested, string background[]) {
	pair <int, int> suggested1 = currSuggested.second.first;
	pair <int, int> suggested2 = currSuggested.second.second;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 10; col++) {
			int x = boardX + col * 5;
			int y = boardY + row * 3;
			if (board[row][col].state != DELETED) {
				if (board[row][col].state == SELECTED) {
					System::setBGandText(RED, BLACK);
					System::drawRectangle(x, y, 5, 3);
					System::gotoXY(x + 1, y + 1);
					cout << " " << (char)board[row][col].animal << " ";
					System::setBGandText(BRIGHT_WHITE, BLACK);
				}
				else {
					if (make_pair(row, col) == currCell) {
						System::setBGandText(GREEN, BLACK);
						System::drawRectangle(x, y, 5, 3);
						System::gotoXY(x + 1, y + 1);
						cout << " " << (char)board[row][col].animal << " ";
						System::setBGandText(BRIGHT_WHITE, BLACK);
					}
					else if (make_pair(row, col) == suggested1 || make_pair(row, col) == suggested2) {
						System::setBGandText(YELLOW, BLACK);
						System::drawRectangle(x, y, 5, 3);
						System::gotoXY(x + 1, y + 1);
						cout << " " << (char)board[row][col].animal << " ";
						System::setBGandText(BRIGHT_WHITE, BLACK);
					}
					else {
						System::setBGandText(BRIGHT_WHITE, BLACK);
						System::drawRectangle(x, y, 5, 3);
						System::gotoXY(x + 1, y + 1);
						cout << " " << (char)board[row][col].animal << " ";
					}
				}
			}
			else {
				if (row == 0 || row == 7 || col == 0 || col == 9)
					continue;
				if (make_pair(row, col) == currCell) {
					System::setBGandText(GREEN, BLACK);
				}
				int pos = 0;
				for (int i = row * 3 - 3; i <= row * 3 - 1; i++) {
					System::gotoXY(x, y + pos);
					for (int j = col * 5 - 5; j <= col * 5 - 1; j++)
						cout << background[i][j];
					pos++;
				}
				System::setBGandText(BRIGHT_WHITE, BLACK);
			}
		}
	}
}

void Board::createBoardList(pair <int, int> currCell, pair <bool, pair <pair <int, int>, pair <int, int>>> currSuggested, string background[]) {
	pair <int, int> suggested1 = currSuggested.second.first;
	pair <int, int> suggested2 = currSuggested.second.second;
	for (int row = 0; row < 8; row++) {
		Node* p = boardList[row];
		int col = 0;
		while (p) {
			int x = boardX + col * 5;
			int y = boardY + row * 3;
			p->data.x = x;
			p->data.y = y;
			if (p->data.state != DELETED) {
				if (p->data.state == SELECTED) {
					System::setBGandText(RED, BLACK);
					System::drawRectangle(x, y, 5, 3);
					System::gotoXY(x + 1, y + 1);
					cout << " " << (char)p->data.animal << " ";
					System::setBGandText(BRIGHT_WHITE, BLACK);
				}
				else {
					if (make_pair(row, col) == currCell) {
						System::setBGandText(GREEN, BLACK);
						System::drawRectangle(x, y, 5, 3);
						System::gotoXY(x + 1, y + 1);
						cout << " " << (char)p->data.animal << " ";
						System::setBGandText(BRIGHT_WHITE, BLACK);
					}
					else if (make_pair(row, col) == suggested1 || make_pair(row, col) == suggested2) {
						if (currSuggested.first) {
							System::setBGandText(YELLOW, BLACK);
							System::drawRectangle(x, y, 5, 3);
							System::gotoXY(x + 1, y + 1);
							cout << " " << (char)p->data.animal << " ";
							System::setBGandText(BRIGHT_WHITE, BLACK);
						}
					}
					else {
						System::setBGandText(BRIGHT_WHITE, BLACK);
						System::drawRectangle(x, y, 5, 3);
						System::gotoXY(x + 1, y + 1);
						cout << " " << (char)p->data.animal << " ";
					}
				}
			}
			else {
				if (row != 0 && row != 7 && col != 0 && col != 9) {
					if (make_pair(row, col) == currCell) {
						System::setBGandText(GREEN, BLACK);
					}
					int pos = 0;
					for (int i = row * 3 - 3; i <= row * 3 - 1; i++) {
						System::gotoXY(x, y + pos);
						for (int j = col * 5 - 5; j <= col * 5 - 1; j++)
							cout << background[i][j];
						pos++;
					}
					System::setBGandText(BRIGHT_WHITE, BLACK);
				}
			}
			col++;
			p = p->pNext;
		}
	}
}

bool Board::CheckLineI(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	// same
	if (Selected_Block_1.first == Selected_Block_2.first && Selected_Block_1.second == Selected_Block_2.second)
		return 0;
	// check if y1 = y2
	if (Selected_Block_1.second == Selected_Block_2.second) {
		if (Selected_Block_1.first > Selected_Block_2.first)
			swap(Selected_Block_1, Selected_Block_2);

		if (Selected_Block_1.first + 1 == Selected_Block_2.first)
			return 1;

		for (int i = Selected_Block_1.first + 1; i < Selected_Block_2.first; i++) {
			if (board[i][Selected_Block_1.second].state != DELETED)
				return 0;
		}
		return 1;
	}
	// Check if x1 = x2
	if (Selected_Block_1.first == Selected_Block_2.first) {
		if (Selected_Block_1.second > Selected_Block_2.second)
			swap(Selected_Block_1, Selected_Block_2);

		if (Selected_Block_1.second + 1 == Selected_Block_2.second)
			return 1;

		for (int i = Selected_Block_1.second + 1; i < Selected_Block_2.second; i++) {
			if (board[Selected_Block_1.first][i].state != DELETED)
				return 0;
		}
		return 1;
	}
	return 0;
}
bool Board::CheckLineL(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	if (Selected_Block_1.first > Selected_Block_2.first)
		swap(Selected_Block_1, Selected_Block_2);

	pair<int, int>Corner;
	Corner = { Selected_Block_1.first, Selected_Block_2.second };
	if (board[Corner.first][Corner.second].state == DELETED && 
		CheckLineI(Selected_Block_1, Corner) && CheckLineI(Corner, Selected_Block_2)) {
		saveAngle = Corner;
		return 1;
	}
	Corner = { Selected_Block_2.first, Selected_Block_1.second };
	if (board[Corner.first][Corner.second].state == DELETED && 
		CheckLineI(Selected_Block_1, Corner) && CheckLineI(Corner, Selected_Block_2)) {
		saveAngle = Corner;
		return 1;
	}
	return 0;
}

bool Board::CheckLineZ(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	pair<int, int>Corner1;
	pair<int, int>Corner2;
	if (Selected_Block_1.first > Selected_Block_2.first)
		swap(Selected_Block_1, Selected_Block_2);

	for (int i = Selected_Block_1.first + 1; i < Selected_Block_2.first; i++) {
		Corner1 = { i, Selected_Block_1.second };
		Corner2 = { i, Selected_Block_2.second };
		if (board[Corner1.first][Corner1.second].state != DELETED || board[Corner2.first][Corner2.second].state != DELETED)
			continue;
		if (CheckLineI(Selected_Block_1, Corner1) && CheckLineI(Corner1, Corner2) && CheckLineI(Corner2, Selected_Block_2)) {
			saveCorner1 = Corner1;
			saveCorner2 = Corner2;
			return 1;
		}
	}

	if (Selected_Block_2.second > Selected_Block_1.second) {
		for (int i = Selected_Block_2.second - 1; i > Selected_Block_1.second; i--) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (board[Corner1.first][Corner1.second].state != DELETED || board[Corner2.first][Corner2.second].state != DELETED)
				continue;
			if (CheckLineI(Selected_Block_1, Corner1) && CheckLineI(Corner1, Corner2) && CheckLineI(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	else {
		for (int i = Selected_Block_1.second - 1; i > Selected_Block_2.second; i--) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (board[Corner1.first][Corner1.second].state != DELETED || board[Corner2.first][Corner2.second].state != DELETED)
				continue;
			if (CheckLineI(Selected_Block_1, Corner1) && CheckLineI(Corner1, Corner2) && CheckLineI(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	return 0;
}
bool Board::CheckLineU(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	pair<int, int>Corner1;
	pair<int, int>Corner2;

	for (int i = 0; i < 8; i++) {
		if ((i < Selected_Block_1.first && i < Selected_Block_2.first) || (i > Selected_Block_1.first && i > Selected_Block_2.first)) {
			Corner1 = { i, Selected_Block_1.second };
			Corner2 = { i, Selected_Block_2.second };
			if (board[Corner1.first][Corner1.second].state != DELETED ||
				board[Corner2.first][Corner2.second].state != DELETED)
				continue;
			if (CheckLineI(Selected_Block_1, Corner1) && CheckLineI(Corner1, Corner2) && CheckLineI(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if ((i < Selected_Block_1.second && i < Selected_Block_2.second) || (i > Selected_Block_1.second && i > Selected_Block_2.second)) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (board[Corner1.first][Corner1.second].state != DELETED ||
				board[Corner2.first][Corner2.second].state != DELETED)
				continue;
			if (CheckLineI(Selected_Block_1, Corner1) && CheckLineI(Corner1, Corner2) && CheckLineI(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	return 0;
}

bool Board::CheckLineIList(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	// same
	if (Selected_Block_1.first == Selected_Block_2.first && Selected_Block_1.second == Selected_Block_2.second)
		return 0;
	// check vertical
	if (Selected_Block_1.second == Selected_Block_2.second) {
		if (Selected_Block_1.first > Selected_Block_2.first)
			swap(Selected_Block_1, Selected_Block_2);

		if (Selected_Block_1.first + 1 == Selected_Block_2.first)
			return 1;

		for (int i = Selected_Block_1.first + 1; i < Selected_Block_2.first; i++) {
			if (getNodeData(boardList[i], Selected_Block_1.second).state != DELETED) {
				return 0;
			}
		}
		return 1;
	}
	// Check horizontal
	if (Selected_Block_1.first == Selected_Block_2.first) {
		if (Selected_Block_1.second > Selected_Block_2.second)
			swap(Selected_Block_1, Selected_Block_2);

		if (Selected_Block_1.second + 1 == Selected_Block_2.second)
			return 1;

		for (int i = Selected_Block_1.second + 1; i < Selected_Block_2.second; i++) {
			if (getNodeData(boardList[Selected_Block_1.first], i).state != DELETED) {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

bool Board::CheckLineLList(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	if (Selected_Block_1.first > Selected_Block_2.first)
		swap(Selected_Block_1, Selected_Block_2);

	pair<int, int>Corner;
	Corner = { Selected_Block_1.first, Selected_Block_2.second };
	if (getNodeData(boardList[Corner.first], Corner.second).state == DELETED &&
		CheckLineIList(Selected_Block_1, Corner) && CheckLineIList(Corner, Selected_Block_2)) {
		saveAngle = Corner;
		return 1;
	}
	Corner = { Selected_Block_2.first, Selected_Block_1.second };
	if (getNodeData(boardList[Corner.first], Corner.second).state == DELETED &&
		CheckLineIList(Selected_Block_1, Corner) && CheckLineIList(Corner, Selected_Block_2)) {
		saveAngle = Corner;
		return 1;
	}
	return 0;
}

bool Board::CheckLineZList(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	pair<int, int>Corner1;
	pair<int, int>Corner2;
	if (Selected_Block_1.first > Selected_Block_2.first)
		swap(Selected_Block_1, Selected_Block_2);

	for (int i = Selected_Block_1.first + 1; i < Selected_Block_2.first; i++) {
		Corner1 = { i, Selected_Block_1.second };
		Corner2 = { i, Selected_Block_2.second };
		if (getNodeData(boardList[Corner1.first], Corner1.second).state != DELETED || getNodeData(boardList[Corner2.first], Corner2.second).state != DELETED)
			continue;
		if (CheckLineIList(Selected_Block_1, Corner1) && CheckLineIList(Corner1, Corner2) && CheckLineIList(Corner2, Selected_Block_2)) {
			saveCorner1 = Corner1;
			saveCorner2 = Corner2;
			return 1;
		}
	}

	if (Selected_Block_2.second > Selected_Block_1.second) {
		for (int i = Selected_Block_2.second - 1; i > Selected_Block_1.second; i--) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (getNodeData(boardList[Corner1.first], Corner1.second).state != DELETED || getNodeData(boardList[Corner2.first], Corner2.second).state != DELETED)
				continue;
			if (CheckLineIList(Selected_Block_1, Corner1) && CheckLineIList(Corner1, Corner2) && CheckLineIList(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	else {
		for (int i = Selected_Block_1.second - 1; i > Selected_Block_2.second; i--) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (getNodeData(boardList[Corner1.first], Corner1.second).state != DELETED || getNodeData(boardList[Corner2.first], Corner2.second).state != DELETED)
				continue;
			if (CheckLineIList(Selected_Block_1, Corner1) && CheckLineIList(Corner1, Corner2) && CheckLineIList(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	return 0;
}
bool Board::CheckLineUList(pair<int, int>Selected_Block_1, pair<int, int>Selected_Block_2) {
	pair<int, int>Corner1;
	pair<int, int>Corner2;

	for (int i = 0; i < 8; i++) {
		if ((i < Selected_Block_1.first && i < Selected_Block_2.first) || (i > Selected_Block_1.first && i > Selected_Block_2.first)) {
			Corner1 = { i, Selected_Block_1.second };
			Corner2 = { i, Selected_Block_2.second };
			if (getNodeData(boardList[Corner1.first], Corner1.second).state != DELETED ||
				getNodeData(boardList[Corner2.first], Corner2.second).state != DELETED)
				continue;
			if (CheckLineIList(Selected_Block_1, Corner1) && CheckLineIList(Corner1, Corner2) && CheckLineIList(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		if ((i < Selected_Block_1.second && i < Selected_Block_2.second) || (i > Selected_Block_1.second && i > Selected_Block_2.second)) {
			Corner1 = { Selected_Block_1.first, i };
			Corner2 = { Selected_Block_2.first, i };
			if (getNodeData(boardList[Corner1.first], Corner1.second).state != DELETED ||
				getNodeData(boardList[Corner2.first], Corner2.second).state != DELETED)
				continue;
			if (CheckLineIList(Selected_Block_1, Corner1) && CheckLineIList(Corner1, Corner2) && CheckLineIList(Corner2, Selected_Block_2)) {
				saveCorner1 = Corner1;
				saveCorner2 = Corner2;
				return 1;
			}
		}
	}
	return 0;
}
