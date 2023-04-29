#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

COORD Cursor;

struct Cell {
	int x;
	int y;
	int xEnd;
	int yEnd;
};

struct Checker {
	int x;
	int y;
	int xEnd;
	int yEnd;
	int color;
	bool isQuinn;
};

Cell cells[8][8];
Checker checkers[8][8];
int field[8][8];




enum ConsoleColors {
	Black = 0,
	White = 1,
	Blue = 2,
	Green = 3,
	Cyan = 4,
	Red = 5,
	Magenta = 6,
	Brown = 7,
};

COORD dot;

HDC cyan, white, black, grey, blue, red;


void draw() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Rectangle(grey, 0, 0, 1200, 1200);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hConsole, &CursorInfo);
	CursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &CursorInfo);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				Rectangle(white, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
			}
			else {
				Rectangle(black, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				if (field[i][j] == 1) {
					RoundRect(red, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
				else if (field[i][j] == 2) {
					RoundRect(blue, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
			}
		}
	}
}


int main() {
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	HWND hcon = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hcon, &rect);
	MoveWindow(hcon, rect.left, rect.top, 800, 800, TRUE);
	
	
	cyan = GetDC(GetConsoleWindow());
	SelectObject(cyan, CreateSolidBrush(RGB(0, 170, 170)));
	white = GetDC(GetConsoleWindow());
	SelectObject(white, CreateSolidBrush(RGB(255, 255, 255)));
	black = GetDC(GetConsoleWindow());
	SelectObject(black, CreateSolidBrush(RGB(0, 0, 0)));
	
	grey = GetDC(GetConsoleWindow());
	SelectObject(grey, CreateSolidBrush(RGB(192, 192, 192)));

	blue = GetDC(GetConsoleWindow());
	SelectObject(blue, CreateSolidBrush(RGB(0, 0, 255)));

	red = GetDC(GetConsoleWindow());
	SelectObject(red, CreateSolidBrush(RGB(255, 0, 0)));
	
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cells[i][j].x = 40 + j * 80;
			cells[i][j].y = 90 + i * 80;
			cells[i][j].xEnd = 40 + (j + 1) * 80;
			cells[i][j].yEnd = 90 + (i + 1) * 80;
			checkers[i][j].x = cells[i][j].x + 30;
			checkers[i][j].y = cells[i][j].y + 30;
			checkers[i][j].xEnd = cells[i][j].xEnd - 30;
			checkers[i][j].yEnd = cells[i][j].yEnd - 30;

		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2 == 1) {
				field[i][j] = 1;
				field[7 - i][7 - j] = 2;
			}
			else {
				field[i][j] = 0;
			}
		}
	}

	/*for (int i = 0; i < 8; i++) { 
		for (int j = 0; j < 8; j++) {
			cout << field[i][j];
		}
		cout << endl;
	}*/

	while (TRUE) {

		draw();
		if (_getch() == 13) {
			system("cls");
			cin.ignore();
			return 0;
		}
	}
}