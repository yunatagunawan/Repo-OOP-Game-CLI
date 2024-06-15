// =========== Game 2.0 =========== //
// fixed: hapus bom setelah meledak //
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

#define maxm 10
#define maxadvm 10
const int startpoint = 10;
const int endpoint = 90;
const int height = 20;
const int width = 100;

using namespace std;

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class CObject {
private:
	int _x, _y, _width, _height;
	char _drawchar;
public:
	CObject() {}

	CObject(int x, int y, int w, int h, char c) : _x(x), _y(y), _width(w), _height(h), _drawchar(c) {}

	int getx() { return _x; }

	int gety() { return _y; }

	int getwidth() { return _width; }

	int getheight() { return _height; }

	void setx(int num) { _x = num; }

	void sety(int num) { _y = num; }

	void setwidth(int num) { _width = num; }

	void setheight(int num) { _height = num; }

	char getdc() { return _drawchar; }

	void setdc(char c) { _drawchar = c; }

	void drawxy(int x, int y) {
		gotoxy(x, y); cout << _drawchar;
	}
};

class CMovableObject : virtual public CObject {
private:
	int _dx, _dy, _dir;
public:
	CMovableObject() {}

	CMovableObject(int x, int y, int w, int h, char c, int dx, int dy, int dir) : CObject(x, y, w, h, c) {
		_dx = dx; _dy = dy; _dir = dir;
	}

	int getdx() { return _dx; }

	int getdy() { return _dy; }

	int getdir() { return _dir; }

	void setdx(int num) { _dx = num; }

	void setdy(int num) { _dy = num; }

	void setdir(int num) { _dir = num; }

	void moveup() {
		setx(getdx()); sety(getdy());
		setdx(getdx()); setdy(getdy() - 1);
	}

	void moveright() {
		setx(getdx()); sety(getdy());
		setdx(getdx() + 1); setdy(getdy());
	}

	void movedown() {
		setx(getdx()); sety(getdy());
		setdx(getdx()); setdy(getdy() + 1);
	}

	void moveleft() {
		setx(getdx()); sety(getdy());
		setdx(getdx() - 1); setdy(getdy());
	}

};

class CPlayer : public CMovableObject {
private:
	int _hit_point;
public:
	CPlayer() {}

	CPlayer(int x, int y, int w, int h, char c, int dx, int dy, int dir, int hp) : CObject(x, y, w, h, c), CMovableObject(x, y, w, h, c, dx, dy, dir) {
		_hit_point = hp;
	}

	void setall(int x, int y, int w, int h, char c, int dx, int dy, int dir, int hp) {
		setx(x);
		sety(y);
		setwidth(w);
		setheight(h);
		setdc(c);
		setdx(dx);
		setdy(dy);
		setdir(dir);
		_hit_point = hp;
	}

	void sethp(int num) { _hit_point = num; }

	int gethp() { return _hit_point; }

	void move(int dir) {
		switch (dir) {
		case 1:
			setdir(1);
			break;
		case 2:
			setdir(2);
			break;
		case 3:
			setdir(3);
			break;
		case 4:
			setdir(4);
			break;
		case 5:
			setdir(5);
			break;
		}
	}

	void updates() {
		switch (getdir()) {
		case 1:
			moveup();
			break;
		case 2:
			moveright();
			break;
		case 3:
			movedown();
			break;
		case 4:
			moveleft();
			break;
		case 5:
			break;
		}
		switch (getdir()) { // to stop if player reach wall
		case 1:
			if (getdy() <= 1) {
				move(5);
				setdy(1);
			}
			break;
		case 2:
			if (getdx() + getwidth() >= width - 1) {
				move(5);
				setdx(width - getwidth());
			}
			break;
		case 3:
			if (getdy() + getheight() >= height - 1) {
				move(5);
				setdy(height - getheight());
			}
			break;
		case 4:
			if (getdx() <= 1) {
				move(5);
				setdx(1);
			}
			break;
		}
	}

	void draw() {
		for (int i = 0; i < getheight(); i++) {
			for (int j = 0; j < getwidth(); j++) {
				if (i == getheight() / 2 && j == getwidth() / 2) {
					gotoxy(getdx() + j, getdy() + i); cout << _hit_point;
				}
				else {
					gotoxy(getdx() + j, getdy() + i); cout << getdc();
				}
			}
		}
	}
};

class CExplodingObject : virtual public CObject {
private:
	int _damage;
	bool _alreadyactive;
public:
	CExplodingObject() {}

	CExplodingObject(int x, int y, int w, int h, char c, int dmg, bool aa) : CObject(x, y, w, h, c) {
		_damage = dmg;
		_alreadyactive = aa;
	}

	void setall(int x, int y, int w, int h, char c, int dmg, bool aa) {
		setx(x);
		sety(y);
		setwidth(w);
		setheight(h);
		setdc(c);
		_damage = dmg;
		_alreadyactive = aa;
	}

	void setdamage(int num) { _damage = num; }

	void setstatus(bool aa) {
		_alreadyactive = aa;
	}

	bool getstatus() { return _alreadyactive; }

	int getdamage() { return _damage; }

	void explode(CPlayer& player) {
		int tx = getx() - 2, ty = gety() - 2;
		for (int i = 0; i < getheight() + 4; i++) {
			for (int j = 0; j < getwidth() + 4; j++) {
				gotoxy(tx + j, ty + i); cout << "#";
			}
		}
		player.sethp(player.gethp() - _damage);
		setdc(' ');
		setdamage(0);
		_alreadyactive = true;
	}

	void draw() {
		for (int i = 0; i < getheight(); i++) {
			for (int j = 0; j < getwidth(); j++) {
				if (i == getheight() / 2 && j == getwidth() / 2) {
					if (!_alreadyactive) {
						gotoxy(getx() + j, gety() + i); cout << _damage;
					}
					else {
						gotoxy(getx() + j, gety() + i); cout << " ";
					}
				}
				else {
					gotoxy(getx() + j, gety() + i); cout << getdc();
				}
			}
		}
	}
};

class CMovingExplodingObject : public CMovableObject, public CExplodingObject {
public:
	CMovingExplodingObject() {}

	CMovingExplodingObject(int x, int y, int w, int h, char c, int dx, int dy, int dir, int dmg, bool aa) : CObject(x, y, w, h, c), CMovableObject(x, y, w, h, c, dx, dy, dir), CExplodingObject(x, y, w, h, c, dmg, aa) {}

	void setall(int x, int y, int w, int h, char c, int dx, int dy, int dir, int dmg, bool aa) {
		setx(x);
		sety(y);
		setwidth(w);
		setheight(h);
		setdc(c);
		setdx(dx);
		setdy(dy);
		setdir(dir);
		setdamage(dmg);
		setstatus(aa);
	}

	void updates() {
		switch (getdir()) {
		case 1:
			moveup();
			break;
		case 2:
			moveright();
			break;
		case 3:
			movedown();
			break;
		case 4:
			moveleft();
			break;
		}

		if (getx() + getwidth() >= endpoint - 1) {
			setdir(4);
		}
		if (getx() <= startpoint + 2) {
			setdir(2);
		}
		if (gety() <= 2) {
			setdir(3);
		}
		if (gety() + getheight() >= height - 1) {
			setdir(1);
		}
	}
};

class CGame {
private:
	bool over, win;
	CPlayer player;
	CExplodingObject mine[maxm];
	CMovingExplodingObject advmine[maxadvm];
public:

	CGame() {}

	void updatesadvmine() {
		for (int i = 0; i < maxadvm; i++) {
			advmine[i].updates();
		}
	}

	void drawmine() {
		for (int i = 0; i < maxm; i++) {
			mine[i].draw();
		}
	}

	void drawadvmine() {
		for (int i = 0; i < maxadvm; i++) {
			advmine[i].draw();
		}
	}

	void checkmine() {
		for (int i = 0; i < maxm; i++) {
			if ((mine[i].getstatus() == false) && (player.getdx() >= mine[i].getx() && player.getdx() <= mine[i].getx() + mine[i].getwidth() - 1 && player.getdy() + player.getheight() - 1 >= mine[i].gety() && player.getdy() + player.getheight() - 1 <= mine[i].gety() + mine[i].getheight() - 1)) {
				mine[i].explode(player);
			}

			if ((mine[i].getstatus() == false) && (player.getdx() + player.getwidth() - 1 >= mine[i].getx() && player.getdx() + player.getwidth() - 1 <= mine[i].getx() + mine[i].getwidth() - 1 && player.getdy() >= mine[i].gety() && player.getdy() <= mine[i].gety() + mine[i].getheight() - 1)) {
				mine[i].explode(player);
			}

			if ((mine[i].getstatus() == false) && (player.getdx() >= mine[i].getx() && player.getdx() <= mine[i].getx() + mine[i].getwidth() - 1 && player.getdy() + player.getheight() - 1 >= mine[i].gety() && player.getdy() + player.getheight() - 1 <= mine[i].gety() + mine[i].getheight() - 1)) {
				mine[i].explode(player);
			}

			if ((mine[i].getstatus() == false) && (player.getdx() + player.getwidth() - 1 >= mine[i].getx() && player.getdx() + player.getwidth() - 1 <= mine[i].getx() + mine[i].getwidth() - 1 && player.getdy() >= mine[i].gety() && player.getdy() <= mine[i].gety() + mine[i].getheight() - 1)) {
				mine[i].explode(player);
			}
		}
	}

	void checkadvmine() {
		for (int i = 0; i < maxadvm; i++) {
			if ((advmine[i].getstatus() == false) && (player.getdx() >= advmine[i].getx() && player.getdx() <= advmine[i].getx() + advmine[i].getwidth() - 1 && player.getdy() + player.getheight() - 1 >= advmine[i].gety() && player.getdy() + player.getheight() - 1 <= advmine[i].gety() + advmine[i].getheight() - 1)) {
				advmine[i].explode(player);
			}
			if ((advmine[i].getstatus() == false) && (player.getdx() + player.getwidth() - 1 >= advmine[i].getx() && player.getdx() + player.getwidth() - 1 <= advmine[i].getx() + advmine[i].getwidth() - 1 && player.getdy() >= advmine[i].gety() && player.getdy() <= advmine[i].gety() + advmine[i].getheight() - 1)) {
				advmine[i].explode(player);
			}

			if ((advmine[i].getstatus() == false) && (player.getdx() >= advmine[i].getx() && player.getdx() <= advmine[i].getx() + advmine[i].getwidth() - 1 && player.getdy() + player.getheight() - 1 >= advmine[i].gety() && player.getdy() + player.getheight() - 1 <= advmine[i].gety() + advmine[i].getheight() - 1)) {
				advmine[i].explode(player);
			}

			if ((advmine[i].getstatus() == false) && (player.getdx() + player.getwidth() - 1 >= advmine[i].getx() && player.getdx() + player.getwidth() - 1 <= advmine[i].getx() + advmine[i].getwidth() - 1 && player.getdy() >= advmine[i].gety() && player.getdy() <= advmine[i].gety() + advmine[i].getheight() - 1)) {
				advmine[i].explode(player);
			}
		}
	}

	void reachend() {
		if (player.getdx() == width - 5 || player.getx() == width - 5) {
			over = true;
			win = true;
		}
	}

	void Initialize() {
		over = false;
		win = false;
		player.setall(3, 3, 3, 3, '+', 3, 3, 5, 10);
		mine[0].setall(15, 3, 5, 5, '0', 10, false);
		mine[1].setall(19, 11, 5, 5, '0', 10, false);
		mine[2].setall(34, 14, 5, 5, '0', 10, false);
		mine[3].setall(43, 5, 5, 5, '0', 10, false);
		mine[4].setall(52, 2, 5, 5, '0', 10, false);
		mine[5].setall(52, 9, 5, 5, '0', 10, false);
		mine[6].setall(64, 6, 5, 5, '0', 10, false);
		mine[7].setall(64, 13, 5, 5, '0', 10, false);
		mine[8].setall(74, 3, 5, 5, '0', 10, false);
		mine[9].setall(76, 10, 5, 5, '0', 10, false);
		advmine[0].setall(13, 17, 3, 3, '*', 13, 17, 3, 4, false);
		advmine[1].setall(25, 9, 3, 3, '*', 25, 9, 2, 8, false);
		advmine[2].setall(28, 3, 3, 3, '*', 28, 3, 1, 7, false);
		advmine[3].setall(45, 17, 3, 3, '*', 45, 17, 3, 4, false);
		advmine[4].setall(17, 15, 3, 3, '*', 17, 15, 2, 5, false);
		advmine[5].setall(75, 3, 3, 3, '*', 75, 3, 4, 6, false);
		advmine[6].setall(75, 3, 3, 3, '*', 75, 3, 4, 9, false);
		advmine[7].setall(85, 3, 3, 3, '*', 85, 3, 3, 8, false);
		advmine[8].setall(85, 16, 3, 3, '*', 85, 16, 1, 5, false);
		advmine[9].setall(52, 12, 3, 3, '*', 52, 12, 4, 7, false);
	}

	void Draw() {
		system("CLS");
		start();
		drawmap();
		drawadvmine();
		drawmine();
		player.draw();
		finish();
	}

	void ProcessInput() {
		if (_kbhit()) {
			switch (_getch()) {
			case 'w': // up
				player.move(1);
				break;
			case 'd': // right
				player.move(2);
				break;
			case 's': // down
				player.move(3);
				break;
			case 'a': // left
				player.move(4);
				break;
			case 'x': // stop
				player.move(5);
				break;
			case 'z': // exit game
				over = true;
				break;
			}
		}
	}

	void Updates() {
		updatesadvmine();
		checkmine();
		checkadvmine();
		player.updates();
		reachend();
	}

	void start() {
		gotoxy(2, 1); cout << "start";
	}

	void finish() {
		gotoxy(93, 1); cout << "finish";
	}

	void End() {
		if (win) {
			system("CLS");
			gotoxy(1, 1); cout << "You Win!";
		}
		else {
			system("CLS");
			gotoxy(1, 1); cout << "You Lose!";
		}
	}

	void hp() {
		if (player.gethp() < 1) {
			over = true;
		}
	}

	void Run() {
		while (!over) {
			Draw();
			ProcessInput();
			Sleep(10);
			Updates();
			hp();
		}
		End();
	}

	void drawmap() {
		for (int i = 0; i <= height; i++) {
			for (int j = 0; j <= width; j++) {
				if (i == 0 || i == height) {
					gotoxy(j, i); cout << "-";
				}
				else if (j == 0 || j == width) {
					gotoxy(j, i); cout << "|";
				}
				else if (j == startpoint || j == endpoint) {
					gotoxy(j, i); cout << "#";
				}
			}
		}
	}
};

void rules() {
	system("CLS");
	cout << "Rules:" << endl;
	cout << "1. Health pemain adalah 10" << endl;
	cout << "2. Instant death apabila mengijak ranjau diam" << endl;
	cout << "3. Health pemain akan berkurang sesuai dengan nomor ditengah ranjau bergerak" << endl;
	cout << "4. Pemain harus menyeberangi lahan jebakan, menang apabila sampai diseberang lahan" << endl;
	cout << "5. Tekan awsd untuk bergerak, x untuk stop move, z untuk quit game" << endl;
	cout << "6. Ranjau yang sudah meledak tidak memiliki character '*' disekitar nilainya dan nilainya 0" << endl;
	cout << "7. Have Fun Playing!!" << endl;
	Sleep(5000); //
}

int main() {
	rules();
	CGame game;
	game.Initialize();
	game.Run();
	return 0;
}