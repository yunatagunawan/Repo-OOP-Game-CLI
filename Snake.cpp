// Snake Game
// Rules:
// 1. Map is not Displayed
// 2. Press 'x' To Exit
// 3. Your Snake Head Will Be '+'
// 4. Modify MAXSIZE to Increase or Decrease Difficulty

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

#define MAXSIZE 20
using namespace std;
const int width = 40;
const int height = 20;
void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class CSegment {
private:
	int _x, _y;
	char _alfabet;
	int lx, ly;
public:
	CSegment() {}
	CSegment(int x, int y, char alfabet = rand() % 25 + 65) : _x(x), _y(y), _alfabet(alfabet), lx(x), ly(y) {}
	void setCurrpos(int x, int y) {
		_x = x; _y = y;
	}
	void setLastpos(int x, int y) {
		lx = x; ly = y;
	}
	void setALL(int x, int y, char alfabet) {
		_x = x;
		_y = y;
		_alfabet = alfabet;
	}
	void setX(int x) {
		_x = x;
	}
	void setY(int y) {
		_y = y;
	}
	void setA(char a) {
		_alfabet = a;
	}
	int getX() {
		return _x;
	}
	int getY() {
		return _y;
	}
	int getlX() {
		return lx;
	}
	int getlY() {
		return ly;
	}
	char getA() {
		return _alfabet;
	}
	void Draw() {
		gotoxy(_x, _y); cout << _alfabet;
	}
	void Erase() {
		gotoxy(_x, _y); cout << " ";
	}
	void MoveUp() {
		_y++;
	}
	void MoveDown() {
		_y--;
	}
	void MoveRight() {
		_x++;
	}
	void MoveLeft() {
		_x--;
	}
};

class CFood {
	int _x, _y, _DX, _DY;
	char _alfabet;
	int Fdir;
public:
	CFood() : _x(rand() % 32 + 2), _DX(1), _y(rand() % 12 + 2), _DY(1), _alfabet(rand() % 25 + 65) {}
	void Set(int x = rand() % 32 + 2, int y = rand() % 12 + 2, char alfabet = rand() % 25 + 65) {
		_x = x;
		_DX = x;
		_y = y;
		_DY = y;
		_alfabet = alfabet;
	}
	int getX() {
		return _x;
	}
	int getY() {
		return _y;
	}
	int getDX() {
		return _DX;
	}
	int getDY() {
		return _DY;
	}
	char getA() {
		return _alfabet;
	}
	void Draw() {
		gotoxy(_x, _y); cout << _alfabet;
	}
	void setDirection(int direction = rand() % 4 + 1) {
		switch (direction) {
		case 1:
			Fdir = 1;
			break;
		case 2:
			Fdir = 2;
			break;
		case 3:
			Fdir = 3;
			break;
		case 4:
			Fdir = 4;
			break;
		}
	}
	void Updates() {
		switch (Fdir) {
		case 1:
			_y = _DY;
			_DY++;
			break;
		case 2:
			_x = _DX;
			_DX++;
			break;
		case 3:
			_y = _DY;
			_DY--;
			break;
		case 4:
			_x = _DX;
			_DX--;
			break;
		}
		if (_DX == width) {
			Fdir = 4;
		}
		if (_DX == 0) {
			Fdir = 2;
		}
		if (_DY == height) {
			Fdir = 3;
		}
		if (_DY == 0) {
			Fdir = 1;
		}
	}
};

class CSnake {
private:
	CSegment* _body;
	int _len, Pdir;
	int score;
public:
	CSnake(int len = 1, int sc = 0) {
		_len = len;
		score = sc;
		_body = new CSegment[MAXSIZE];
		_body[0].setALL(20, 10, '+');
	}
	~CSnake() {
		delete[]_body;
	}
	int getScore() {
		return score;
	}
	CSegment getSeg(int index) {
		return _body[index];
	}
	CSegment operator [](int index) const {
		return _body[index];
	}
	CSegment& operator [] (int index) {
		return _body[index];
	}
	void eatFood(CFood& food) {
		_body[_len].setA(food.getA());
		_len++; score++;
	}
	void Move(int direction) {

		switch (direction) {
		case 1:
			Pdir = 1;
			break;
		case 2:
			Pdir = 2;
			break;
		case 3:
			Pdir = 3;
			break;
		case 4:
			Pdir = 4;
			break;
		}

	}
	void Draw() {
		for (int i = 0; i < _len; i++) {
			_body[i].Draw();
		}
	}
	void Updates() {

		_body[0].setLastpos(_body[0].getX(), _body[0].getY());

		switch (Pdir) {
		case 3:
			_body[0].MoveUp();
			break;
		case 2:
			_body[0].MoveRight();
			break;
		case 1:
			_body[0].MoveDown();
			break;
		case 4:
			_body[0].MoveLeft();
			break;
		}

		for (int i = 1; i < _len; i++) {
			_body[i].setLastpos(_body[i].getX(), _body[i].getY());
			_body[i].setCurrpos(_body[i - 1].getlX(), _body[i - 1].getlY());
		}

	}
};

class CGameSnake {
	CSnake player;
	CFood food[5];
	bool GameOver = false;
public:
	void Initialize() {
		for (int i = 0; i < 5; i++) {
			food[i].Set();
			food[i].setDirection(rand() % 4 + 1);
		}
	}
	void Draw() {
		system("CLS");
		food[0].Draw();
		food[1].Draw();
		food[2].Draw();
		food[3].Draw();
		food[4].Draw();
		player.Draw();
	}
	void ProcessInput() {
		if (_kbhit()) {
			switch (_getch()) {
			case 'w':
				player.Move(1);
				break;
			case 'd':
				player.Move(2);
				break;
			case 's':
				player.Move(3);
				break;
			case 'a':
				player.Move(4);
				break;
			case 'x':
				GameOver = true;
				break;
			}
		}
	}
	void check(CSnake& player, CFood& food) {
		if (player[0].getX() == food.getX() && player[0].getY() == food.getY() || player[0].getX() == food.getDX() && player[0].getY() == food.getDY()) {
			player.eatFood(food);
			food.setDirection();
			food.Set();
		}
	}
	void hitWall(CSnake& player) {
		if ((player[0].getX() == 0 || player[0].getX() == width || player[0].getY() == 0 || player[0].getY() == height) || player.getScore() == MAXSIZE)
			GameOver = true;
	}
	void Updates() {
		check(player, food[0]);
		check(player, food[1]);
		check(player, food[2]);
		check(player, food[3]);
		check(player, food[4]);
		food[0].Updates();
		food[1].Updates();
		food[2].Updates();
		food[3].Updates();
		food[4].Updates();
		player.Updates();
		hitWall(player);
	}
	void Run() {
		while (!GameOver) {
			Draw();
			ProcessInput();
			Sleep(10);
			Updates();
		}
	}
	void Score(CSnake& player) {
		system("CLS");
		gotoxy(1, 1); cout << "Max Score: " << MAXSIZE << endl;
		gotoxy(1, 2); cout << "Your Score: " << player.getScore() << endl;
		if (player.getScore() == MAXSIZE) {
			gotoxy(1, 3); cout << "You Win!!" << endl;
		}
		else {
			gotoxy(1, 3); cout << "You Lose!!" << endl;
		}

	}
	void End() {
		Score(player);
	}
};

int main() {
	srand(time(NULL));
	CGameSnake game;
	game.Initialize();
	game.Run();
	game.End();
	return EXIT_SUCCESS;
}