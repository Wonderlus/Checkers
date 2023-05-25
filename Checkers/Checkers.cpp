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
	int checker;
	bool isChosen = false;
	bool isDead = true;

};

struct Checker {
	int x;
	int y;
	int xEnd;
	int yEnd;
	int color;
};

Cell cells[8][8];
Checker checkers[8][8];

// Используется для отрисовки
// 0 - пустая клетка с шашкой или без
// 1 - текущее положение курсора
// 2 - возможные ходы(обнуляется, если отменяем ход)
// 3 - курсор на клетке с возможным ходом
// 5 - выбранная клетка
int field[8][8];

// Текущий ход
int turn = 2;


// Позиция в которой находится шашка, которая будет съедена
int deadMoveY = 0;
int deadMoveX = 0;

// Ход компьютера
int computerMoveY = 0;
int computerMoveX = 0;

// Выбор режима игры
int choiceGame = 0;

// Выбранный режим игры
int chosenGame = 0;

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

HDC cyan, white, black, grey, blue, red, yellow, brown, silver, iron, darkGreen, whiteQueen, blackQueen;





void drawCursor() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (field[i][j] == 1) {
				Rectangle(silver, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				if (cells[i][j].checker == 1) {
					RoundRect(brown, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
				else if (cells[i][j].checker == 3) {
					RoundRect(blackQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 2) {
					RoundRect(white, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 4) {
					RoundRect(whiteQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

			}
			else if (field[i][j] == 3) {
				Rectangle(silver, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				RoundRect(darkGreen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
			}

			else if (cells[i][j].isChosen) {
				Rectangle(darkGreen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				if (cells[i][j].checker == 1) {
					RoundRect(brown, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
				else if (cells[i][j].checker == 3) {
					RoundRect(blackQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 2) {
					RoundRect(white, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 4) {
					RoundRect(whiteQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
			}

		}
	}
}

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
				if (cells[i][j].isDead) {
					cells[i][j].checker = 0;
				}
				Rectangle(yellow, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				if (field[i][j] == 2) {
					RoundRect(iron, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

			}

			else {
				Rectangle(black, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
				if (field[i][j] == 2) {
					RoundRect(darkGreen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				if (cells[i][j].checker == 1) {
					RoundRect(brown, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
				else if (cells[i][j].checker == 3) {
					RoundRect(blackQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 2) {
					RoundRect(white, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}

				else if (cells[i][j].checker == 4) {
					RoundRect(whiteQueen, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd, 90, 90);
				}
			}

		}
	}
}


bool getDeadMoves(int curPositionX, int curPositionY, int turn) {
	bool flag = false;
	if ((curPositionX - 2) >= 0 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX - 1].checker % 2 != turn % 2)
		and (cells[curPositionY + 1][curPositionX - 1].checker != 0)
		and (turn > 0)
		and (cells[curPositionY + 2][curPositionX - 2].checker == 0)
		) {
		flag = true;
		field[curPositionY + 2][curPositionX - 2] = 2;
	}

	if ((curPositionX + 2) <= 7 and ((curPositionY - 2) >= 0)
		and (cells[curPositionY - 1][curPositionX + 1].checker != turn)
		and (cells[curPositionY - 1][curPositionX + 1].checker > 0)
		and (turn > 0)
		and (cells[curPositionY - 2][curPositionX + 2].checker == 0)
		) {
		flag = true;
		field[curPositionY - 2][curPositionX + 2] = 2;
	}

	if ((curPositionX + 2) <= 7 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX + 1].checker != turn)
		and (cells[curPositionY + 1][curPositionX + 1].checker > 0)
		and (turn > 0)
		and (cells[curPositionY + 2][curPositionX + 2].checker == 0)
		) {
		flag = true;
		field[curPositionY + 2][curPositionX + 2] = 2;
	}

	if ((curPositionX - 2) >= 0 and ((curPositionY - 2) >= 0)
		and (cells[curPositionY - 1][curPositionX - 1].checker != turn)
		and (cells[curPositionY - 1][curPositionX - 1].checker > 0)
		and (turn > 0)
		and (cells[curPositionY - 2][curPositionX - 2].checker == 0)
		) {
		flag = true;
		field[curPositionY - 2][curPositionX - 2] = 2;
	}
	return flag;
}

void getPossibleMoves(int curPositionX, int curPositionY, int turn) {

	if ((curPositionX - 1) >= 0 and ((curPositionY + 1) <= 7)
		and (cells[curPositionY + 1][curPositionX - 1].checker == 0)
		and (turn % 2 == 1)) {
		field[curPositionY + 1][curPositionX - 1] = 2;
	}

	if ((curPositionX + 1) <= 7 and ((curPositionY - 1) >= 0)
		and (cells[curPositionY - 1][curPositionX + 1].checker == 0)
		and (turn % 2 == 0)) {
		field[curPositionY - 1][curPositionX + 1] = 2;
	}

	if ((curPositionX + 1) <= 7 and ((curPositionY + 1) <= 7)
		and (cells[curPositionY + 1][curPositionX + 1].checker == 0)
		and (turn % 2 == 1)) {
		field[curPositionY + 1][curPositionX + 1] = 2;
	}

	if ((curPositionX - 1) >= 0 and ((curPositionY - 1) >= 0)
		and (cells[curPositionY - 1][curPositionX - 1].checker == 0)
		and (turn % 2 == 0)) {
		field[curPositionY - 1][curPositionX - 1] = 2;
	}


	getDeadMoves(curPositionX, curPositionY, turn);
}

void getPossibleMovesQueen(int curPositionX, int curPositionY, int turn) {
	int movePositionX = curPositionX;
	int movePositionY = curPositionY;

	for (int i = 0; i < 8; i++) {
		if (movePositionX >= 7 or movePositionY >= 7) {
			break;
		}

		movePositionX += 1;
		movePositionY += 1;
		if ((cells[movePositionY][movePositionX].checker % 2 == turn % 2) and (cells[movePositionY][movePositionX].checker != 0)) {
			break;
		}

		if (cells[movePositionY][movePositionX].checker == 0) {
			field[movePositionY][movePositionX] = 2;
		}
		
		if ((cells[movePositionY][movePositionX].checker % 2 != turn % 2)
			and deadMoveX == 0 and deadMoveY == 0 
			and cells[movePositionY][movePositionX].checker != 0) {
			deadMoveX = movePositionX;
			deadMoveY = movePositionY;
		}
	}

	movePositionX = curPositionX;
	movePositionY = curPositionY;
	for (int i = 0; i < 8; i++) {
		if (movePositionX <= 0 or movePositionY <= 0) {
			break;
		}

		

		movePositionX -= 1;
		movePositionY -= 1;
		if ((cells[movePositionY][movePositionX].checker % 2 == turn % 2) and (cells[movePositionY][movePositionX].checker != 0)) {
			break;
		}

		if (cells[movePositionY][movePositionX].checker == 0) {
			field[movePositionY][movePositionX] = 2;
		}

		if ((cells[movePositionY][movePositionX].checker % 2 != turn % 2)
			and deadMoveX == 0 and deadMoveY == 0
			and cells[movePositionY][movePositionX].checker != 0) {
			deadMoveX = movePositionX;
			deadMoveY = movePositionY;
		}
	}

	movePositionX = curPositionX;
	movePositionY = curPositionY;
	for (int i = 0; i < 8; i++) {
		if ( movePositionY >= 7 or movePositionX <= 0) {
			break;
		}

		

		movePositionX -= 1;
		movePositionY += 1;
		if ((cells[movePositionY][movePositionX].checker % 2 == turn % 2) and (cells[movePositionY][movePositionX].checker != 0)) {
			break;
		}
		if (cells[movePositionY][movePositionX].checker == 0) {
			field[movePositionY][movePositionX] = 2;
		}

		if ((cells[movePositionY][movePositionX].checker % 2 != turn % 2)
			and deadMoveX == 0 and deadMoveY == 0
			and cells[movePositionY][movePositionX].checker != 0) {
			deadMoveX = movePositionX;
			deadMoveY = movePositionY;
		}
	}

	movePositionX = curPositionX;
	movePositionY = curPositionY;
	for (int i = 0; i < 8; i++) {

		if (movePositionX >= 7 or movePositionY <= 0) {
			break;
		}

		

		movePositionX += 1;
		movePositionY -= 1;
		if ((cells[movePositionY][movePositionX].checker % 2 == turn % 2) and (cells[movePositionY][movePositionX].checker != 0)) {
			break;
		}
		if (cells[movePositionY][movePositionX].checker == 0) {
			field[movePositionY][movePositionX] = 2;
		}

		if ((cells[movePositionY][movePositionX].checker % 2 != turn % 2)
			and deadMoveX == 0 and deadMoveY == 0
			and cells[movePositionY][movePositionX].checker != 0) {
			deadMoveX = movePositionX;
			deadMoveY = movePositionY;
		}
	}

}

void checkQueen() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (((i == 0 and j == 1) or
				(i == 0 and j == 3) or
				(i == 0 and j == 5) or
				(i == 0 and j == 7)) and
				turn == 1 and
				cells[i][j].checker == 2) {
				cells[i][j].checker += 2;
			}

			else if (((i == 7 and j == 0) or
				(i == 7 and j == 2) or
				(i == 7 and j == 4) or
				(i == 7 and j == 6)) and
				turn == 2 and
				cells[i][j].checker == 1) {
				cells[i][j].checker += 2;
			}
		}
	}
}

int findBestMove(int curPositionX, int curPositionY, int turn, int n) {
	int plusScore = 0;

	// Берем дамку
	if ((curPositionX - 2) >= 0 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX - 1].checker == turn % 2 + 3)
		and (cells[curPositionY + 1][curPositionX - 1].checker != 0)
		and (cells[curPositionY + 2][curPositionX - 2].checker == 0)
		) {
		plusScore = 20;
		if (n == 0) {
			computerMoveY = curPositionY + 2;
			computerMoveX = curPositionX - 2;
		}
	}

	else if ((curPositionX + 2) >= 0 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX + 1].checker == turn % 2 + 3)
		and (cells[curPositionY + 1][curPositionX + 1].checker != 0)
		and (cells[curPositionY + 2][curPositionX + 2].checker == 0)
		) {
		plusScore = 20;
		if (n == 0) {
			computerMoveY = curPositionY + 2;
			computerMoveX = curPositionX + 2;
		}
	}

	else if ((curPositionX - 2) >= 0 and ((curPositionY - 2) <= 7)
		and (cells[curPositionY - 1][curPositionX - 1].checker == turn % 2 + 3)
		and (cells[curPositionY - 1][curPositionX - 1].checker != 0)
		and (cells[curPositionY - 2][curPositionX - 2].checker == 0)
		) {
		plusScore = 20;
		if (n == 0) {
			computerMoveY = curPositionY - 2;
			computerMoveX = curPositionX - 2;
		}
	}

	else if ((curPositionX + 2) >= 0 and ((curPositionY - 2) <= 7)
		and (cells[curPositionY - 1][curPositionX + 1].checker == turn % 2 + 3)
		and (cells[curPositionY - 1][curPositionX + 1].checker != 0)
		and (cells[curPositionY - 2][curPositionX + 2].checker == 0)
		) {
		plusScore = 20;
		if (n == 0) {
			computerMoveY = curPositionY - 2;
			computerMoveX = curPositionX + 2;
		}
	}

	// Берем обычную шашку
	if ((curPositionX - 2) >= 0 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX - 1].checker == turn % 2 + 1)
		and (cells[curPositionY + 1][curPositionX - 1].checker != 0)
		and (cells[curPositionY + 2][curPositionX - 2].checker == 0)
		) {
		plusScore = 5;
		if (n == 0) {
			computerMoveY = curPositionY + 2;
			computerMoveX = curPositionX - 2;
		}
	}

	else if ((curPositionX + 2) >= 0 and ((curPositionY + 2) <= 7)
		and (cells[curPositionY + 1][curPositionX + 1].checker == turn % 2 + 1)
		and (cells[curPositionY + 1][curPositionX + 1].checker != 0)
		and (cells[curPositionY + 2][curPositionX + 2].checker == 0)
		) {
		plusScore = 5;
		if (n == 0) {
			computerMoveY = curPositionY + 2;
			computerMoveX = curPositionX + 2;
		}
	}

	else if ((curPositionX - 2) >= 0 and ((curPositionY - 2) <= 7)
		and (cells[curPositionY - 1][curPositionX - 1].checker == turn % 2 + 1)
		and (cells[curPositionY - 1][curPositionX - 1].checker != 0)
		and (cells[curPositionY - 2][curPositionX - 2].checker == 0)
		) {
		plusScore = 5;
		if (n == 0) {
			computerMoveY = curPositionY - 2;
			computerMoveX = curPositionX - 2;
		}
	}

	else if ((curPositionX + 2) >= 0 and ((curPositionY - 2) <= 7)
		and (cells[curPositionY - 1][curPositionX + 1].checker == turn % 2 + 1)
		and (cells[curPositionY - 1][curPositionX + 1].checker != 0)
		and (cells[curPositionY - 2][curPositionX + 2].checker == 0)
		) {
		plusScore = 5;
		if (n == 0) {
			computerMoveY = curPositionY - 2;
			computerMoveX = curPositionX + 2;
		}
	}

	// Обычный ход
	if ((curPositionX - 1) >= 0 and ((curPositionY + 1) <= 7)
		and (cells[curPositionY + 1][curPositionX - 1].checker == 0)
		and turn == 1) {
		plusScore = 1;
		if (n == 0) {
			computerMoveY = curPositionY + 1;
			computerMoveX = curPositionX - 1;
		}
	}

	if ((curPositionX + 1) <= 7 and ((curPositionY - 1) >= 0)
		and (cells[curPositionY - 1][curPositionX + 1].checker == 0)
		and turn == 2) {
		plusScore = 1;
		if (n == 0) {
			computerMoveY = curPositionY - 1;
			computerMoveX = curPositionX + 1;
		}
	}

	if ((curPositionX + 1) <= 7 and ((curPositionY + 1) <= 7)
		and (cells[curPositionY + 1][curPositionX + 1].checker == 0)
		and turn == 1) {
		plusScore = 1;
		if (n == 0) {
			computerMoveY = curPositionY + 1;
			computerMoveX = curPositionX + 1;
		}
	}

	if ((curPositionX - 1) >= 0 and ((curPositionY - 1) >= 0)
		and (cells[curPositionY - 1][curPositionX - 1].checker == 0)
		and turn == 2) {
		plusScore = 1;
		if (n == 0) {
			computerMoveY = curPositionY - 1;
			computerMoveX = curPositionX - 1;
		}
	}


	if (turn == 2) {
		return plusScore;
	}
	else if (turn == 1) {
		return -plusScore;
	}
}







// 3 полухода
int countBestMove(int curPositionX, int curPositionY, int n, int turn, int score) {
	if (n == 3) {
		return score;
	}
	
	score += findBestMove(curPositionX, curPositionY, turn, n);
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (cells[i][j].checker != 0) {
				return countBestMove(j, i, n + 1, turn % 2 + 1, score);
			}
			
		}
	}
	
}


// Игра компьютера
void moveComputer() {
	int scoreBest = -1;
	int bestMoveY = 0;
	int bestMoveX = 0;
	int curPositionY = 0;
	int curPositionX = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int score = 0;
			if (cells[i][j].checker % 2 == turn % 2 and cells[i][j].checker != 0) {
				score = countBestMove(j, i, 0, turn, score);
				if (score > scoreBest) {
					scoreBest = score;
					bestMoveY = computerMoveY;
					bestMoveX = computerMoveX;
					curPositionY = i;
					curPositionX = j;
				}

			}
		}
	}
	
	deadMoveY = (curPositionY - bestMoveY) / 2;
	deadMoveX = (curPositionX - bestMoveX) / 2;

	if (deadMoveX != 0 and deadMoveY != 0) {
		cells[curPositionY - deadMoveY][curPositionX - deadMoveX].checker = 0;
	}
	
	cells[bestMoveY][bestMoveX].checker = cells[curPositionY][curPositionX].checker;
	cells[curPositionY][curPositionX].checker = 0;

	turn = turn % 2 + 1;
}

bool checkWin() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (cells[i][j].checker != 0 and cells[i][j].checker != 1 and cells[i][j].checker != 3) {
				return false;
			}
		}
	}

	return true;
}


void move() {



	int curPositionX = 0;
	int curPositionY = 0;
	int chosenX = curPositionX;
	int chosenY = curPositionY;
	bool entered = false;

	while (TRUE) {
		// ВНИЗ - 80, ВВЕРХ - 72, ВПРАВО - 77, ВЛЕВО - 75, ENTER - 13, ESC - 27.

		if (turn == 2 and chosenGame == 0) {
			break;
		}
		int curMove = _getch();


		if (curMove == 80) {
			bool flag = false;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (field[i][j] == 1 or field[i][j] == 3) {
						if (i < 7) {

							field[i][j] -= 1;
							field[i + 1][j] += 1;
							curPositionX = j;
							curPositionY = i + 1;
						}
						flag = true;
						break;
					}
				}
				if (flag) {
					break;
				}
			}

			draw();
			drawCursor();

		}

		if (curMove == 72) {
			bool flag = false;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (field[i][j] == 1 or field[i][j] == 3) {
						if (i > 0) {
							field[i][j] -= 1;
							field[i - 1][j] += 1;
							curPositionX = j;
							curPositionY = i - 1;
						}
						flag = true;
						break;
					}
				}
				if (flag) {
					break;
				}
			}

			draw();
			drawCursor();
		}



		if (curMove == 77) {
			bool flag = false;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (field[i][j] == 1 or field[i][j] == 3) {
						if (j < 7) {
							field[i][j] -= 1;
							field[i][j + 1] += 1;
							curPositionX = j + 1;
							curPositionY = i;
						}
						flag = true;
						break;
					}
				}
				if (flag) {
					break;
				}
			}

			draw();
			drawCursor();
		}

		if (curMove == 75) {
			bool flag = false;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (field[i][j] == 1 or field[i][j] == 3) {
						if (j > 0) {
							field[i][j] -= 1;
							field[i][j - 1] += 1;
							curPositionX = j - 1;
							curPositionY = i;
						}
						flag = true;
						break;
					}
				}
				if (flag) {
					break;
				}
			}


			draw();
			drawCursor();
		}

		if (curMove == 13) {

			if (!entered and cells[curPositionY][curPositionX].checker % 2 == turn % 2 and cells[curPositionY][curPositionX].checker != 0) {
				entered = true;
				chosenX = curPositionX;
				chosenY = curPositionY;
				cells[chosenY][chosenX].isChosen = true;
				if (cells[chosenY][chosenX].checker == 1 or cells[chosenY][chosenX].checker == 2) {
					getPossibleMoves(curPositionX, curPositionY, turn);
				}

				else if (cells[chosenY][chosenX].checker == 3 or cells[chosenY][chosenX].checker == 4) {
					deadMoveX = 0;
					deadMoveY = 0;
					getPossibleMovesQueen(curPositionX, curPositionY, turn);
				}
				
				draw();
				drawCursor();

			}

			else if (entered) {
				if ((curPositionX == chosenX) and (curPositionY == chosenY)) {
					entered = false;
					cells[chosenY][chosenX].isChosen = false;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							field[i][j] = 0;
						}
					}
					field[curPositionY][curPositionX] = 1;

					draw();
					drawCursor();
				}

				else if (field[curPositionY][curPositionX] == 3) {
					

					if (cells[chosenY][chosenX].checker == 1 or cells[chosenY][chosenX].checker == 2) {
						deadMoveY = (chosenY - curPositionY) / 2;
						deadMoveX = (chosenX - curPositionX) / 2;

						if (deadMoveY != 0 and deadMoveX != 0) {
							cells[chosenY - deadMoveY][chosenX - deadMoveX].checker = 0;
						}
					}

					else if (cells[chosenY][chosenX].checker == 3 or cells[chosenY][chosenX].checker == 4) {
	
						if ((((curPositionY - deadMoveY) < 0 == (curPositionY - chosenY) < 0)
							and ((curPositionX - deadMoveX) < 0 == (curPositionX - chosenX) < 0))
							or
							(((curPositionY - deadMoveY) > 0 == (curPositionY - chosenY) > 0)
								and ((curPositionX - deadMoveX) > 0 == (curPositionX - chosenX) > 0))
							or
							(((curPositionY - deadMoveY) < 0 == (curPositionY - chosenY) < 0)
								and ((curPositionX - deadMoveX) > 0 == (curPositionX - chosenX) > 0))
							or
							(((curPositionY - deadMoveY) > 0 == (curPositionY - chosenY) > 0)
								and ((curPositionX - deadMoveX) < 0 == (curPositionX - chosenX) < 0))

							)
							cells[deadMoveY][deadMoveX].checker = 0;
					}



					
					cells[curPositionY][curPositionX].checker = cells[chosenY][chosenX].checker;
					cells[chosenY][chosenX].checker = 0;
					entered = false;
					cells[chosenY][chosenX].isChosen = false;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							field[i][j] = 0;
						}
					}
					field[curPositionY][curPositionX] = 1;

					if (deadMoveX != 0 and deadMoveY != 0) {
						if (getDeadMoves(curPositionX, curPositionY, turn)) {
							entered = true;
							chosenX = curPositionX;
							chosenY = curPositionY;
							cells[chosenY][chosenX].isChosen = true;


						}

						else {
							turn = turn % 2 + 1;
							
						}
					}

					else {
						turn = turn % 2 + 1;
						
					}


					draw();
					drawCursor();

				}




			}
			
			checkQueen();

		}

		if (curMove == 27) {
			system("cls");
			cin.ignore();
			break;
		}

		
	}


}

void drawWin() {
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND hcon = GetConsoleWindow();
	dot.X = 50;
	dot.Y = 20;
	SetConsoleCursorPosition(hConsole, dot);
	
	system("color F0");
	if (chosenGame == 1) {
		cout << "Выиграл белый игрок";
	}
	else if (chosenGame == 0) {
		cout << "Вы выиграли!";
	}
	
}

void drawInterface() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hConsole, &CursorInfo);
	CursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &CursorInfo);

	Rectangle(grey, 0, 0, 1200, 1200);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 0) {
				Rectangle(yellow, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);

			}

			else {
				Rectangle(black, cells[i][j].x, cells[i][j].y, cells[i][j].xEnd, cells[i][j].yEnd);
			}

		}
	}

	SetLayeredWindowAttributes(console, RGB(255, 255, 255), 236, LWA_ALPHA);

	
	while (TRUE) {
		
		


		if (choiceGame == 0) {
			dot.Y = 1;
			dot.X = 0;
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Игра против компьютера";
			dot.Y = 2;
			dot.X = 0;
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Игра против другого игрока";

		}
		if (choiceGame == 1) {
			dot.Y = 1;
			dot.X = 0;
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Игра против компьютера";
			dot.Y = 2;
			dot.X = 0;
			SetConsoleCursorPosition(hConsole, dot);
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Игра против другого игрока";
		}

		if (chosenGame != 0) {
			break;
		}

		int curMove = _getch();
		if (curMove == 80) {
			choiceGame = 1;
			

		}
		else if (curMove == 72) {
			choiceGame = 0;
		}
			

		else if (curMove == 13) {
			chosenGame = choiceGame;
			break;
		}
		
		SetConsoleCursorPosition(hConsole, dot);

		

		
	}
	
}


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	HWND hcon = GetConsoleWindow();
	RECT rect;
	GetWindowRect(hcon, &rect);
	MoveWindow(hcon, rect.left, rect.top, 800, 800, TRUE);


	cyan = GetDC(GetConsoleWindow());
	SelectObject(cyan, CreateSolidBrush(RGB(0, 170, 170)));
	white = GetDC(GetConsoleWindow());
	SelectObject(white, CreateSolidBrush(RGB(255, 255, 255)));

	whiteQueen = GetDC(GetConsoleWindow());
	SelectObject(whiteQueen, CreateSolidBrush(RGB(247, 250, 192)));

	blackQueen = GetDC(GetConsoleWindow());
	SelectObject(blackQueen, CreateSolidBrush(RGB(97, 76, 60)));

	black = GetDC(GetConsoleWindow());
	SelectObject(black, CreateSolidBrush(RGB(0, 0, 0)));

	grey = GetDC(GetConsoleWindow());
	SelectObject(grey, CreateSolidBrush(RGB(211, 211, 211)));

	silver = GetDC(GetConsoleWindow());
	SelectObject(silver, CreateSolidBrush(RGB(192, 192, 192)));

	blue = GetDC(GetConsoleWindow());
	SelectObject(blue, CreateSolidBrush(RGB(0, 0, 255)));

	red = GetDC(GetConsoleWindow());
	SelectObject(red, CreateSolidBrush(RGB(255, 0, 0)));
	yellow = GetDC(GetConsoleWindow());
	SelectObject(yellow, CreateSolidBrush(RGB(237, 171, 86)));

	brown = GetDC(GetConsoleWindow());
	SelectObject(brown, CreateSolidBrush(RGB(115, 74, 18)));

	iron = GetDC(GetConsoleWindow());
	SelectObject(iron, CreateSolidBrush(RGB(218, 223, 225)));

	darkGreen = GetDC(GetConsoleWindow());
	SelectObject(darkGreen, CreateSolidBrush(RGB(48, 130, 81)));


	// Заполнение координат и цветов для клеток
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

	// Заполнение поля отображения
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			field[i][j] = 0;
		}
	}

	// Начальная позиция курсора
	field[0][0] = 1;
	Rectangle(grey, cells[0][0].x, cells[0][0].y, cells[0][0].xEnd, cells[0][0].yEnd);

	// Заполнение цветов для шашек
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 8; j++) {

			if ((i + j) % 2 == 1) {
				cells[i][j].checker = 1;
				cells[7 - i][7 - j].checker = 2;

			}
			else {
				cells[i][j].checker = 0;
			}
		}
	}

	/*cells[7][2].checker = 2;*/


	drawInterface();
	system("cls");
	draw();
	drawCursor();
	SetLayeredWindowAttributes(GetConsoleWindow(), RGB(255, 255, 255), 255, LWA_ALPHA);
	while (TRUE) {
		
		if (checkWin()) {
			drawWin();
			
		}
		
		else if (turn == 2 and chosenGame == 0) {
			moveComputer();
		}

		else {
			move();
		}
		

	}
}

// оценка ходов
// 1 - обычный ход
// 2 - ход на край доски
// 5 - взятие, суммирование при взятии нескольких шашек
// 10 - получение дамки
// 20 - взятие дамки
// 