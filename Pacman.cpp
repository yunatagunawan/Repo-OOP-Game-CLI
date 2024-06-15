#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <ctime>
#include <cmath>

#define wincon 377 
#define opponent 5
const int width = 30;
const int height = 14;

using namespace std;
using namespace chrono;

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class CTimer {
private:
	time_point<system_clock> m_StartTime;
	time_point<system_clock> m_EndTime;
	bool m_bRunning = false;
public:
	void start() {
		m_StartTime = system_clock::now();
		m_bRunning = true;
	}

	void stop() {
		m_EndTime = system_clock::now();
		m_bRunning = false;
	}

	double elapsedMilliseconds() {
		time_point<system_clock> endTime;

		if (m_bRunning)
		{
			endTime = system_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}

		return duration_cast<milliseconds>(endTime - m_StartTime).count();
	}

	double elapsedSeconds() {
		return elapsedMilliseconds() / 1000.0;
	}
};

class CObject {
private:
	int _x, _y;
	char _c;
public:
	CObject(int x = 0, int y = 0, char c = 0) : _x(x), _y(y), _c(c) {}

	void draw() { gotoxy(_x, _y); cout << _c; }

	int getx() { return _x; }

	int gety() { return _y; }

	char getc() { return _c; }

	void setx(int num) { _x = num; }

	void sety(int num) { _y = num; }

	void setc(char chr) { _c = chr; }
};

class CFood : public CObject {
public:
	void set(int x, int y, char chr = '.') {
		this->setx(x); this->sety(y); this->setc(chr);
	}
};

class CMovableObject : public CObject {
private:
	int _lx, _ly, _dir;
public:
	CMovableObject(int x, int y, char c, int lx, int ly, int dir) : CObject(x, y, c) {
		_lx = lx, _ly = ly; _dir = dir;
	}

	int getlx() { return _lx; }

	int getly() { return _ly; }

	int getdir() { return _dir; }

	void setlx(int num) { _lx = num; }

	void setly(int num) { _ly = num; }

	void setdir(char dir) { _dir = dir; }

	void moveup() {
		this->setlx(this->getx()); this->setly(this->gety());
		this->setx(this->getx()); this->sety(this->gety() - 1);
	}

	void moveright() {
		this->setlx(this->getx()); this->setly(this->gety());
		this->setx(this->getx() + 1); this->sety(this->gety());
	}

	void movedown() {
		this->setlx(this->getx()); this->setly(this->gety());
		this->setx(this->getx()); this->sety(this->gety() + 1);
	}

	void moveleft() {
		this->setlx(this->getx()); this->setly(this->gety());
		this->setx(this->getx() - 1); this->sety(this->gety());
	}
};

class CPlayer : public CMovableObject {
	int ChargePhase, PowerBar;
	bool PhaseStatus;
public:
	CPlayer(int x = width / 2, int y = height / 2, char c = '$', int lx = 5, int ly = 5, int dir = 0, int cp = 0, int pb = 0, bool ps = false) : CMovableObject(x, y, c, lx, ly, dir) {
		ChargePhase = cp; PowerBar = pb; PhaseStatus = ps;
	}

	void move(int dir) {
		switch (dir) {
		case 1:
			this->setdir(1);
			break;
		case 2:
			this->setdir(2);
			break;
		case 3:
			this->setdir(3);
			break;
		case 4:
			this->setdir(4);
			break;
		case 5:
			this->setdir(5);
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
			if (gety() <= 1) {
				move(5);
				sety(1);
			}
			break;
		case 2:
			if (getx() >= width - 1) {
				move(5);
				setx(width - 1);
			}
			break;
		case 3:
			if (gety() >= height - 1) {
				move(5);
				sety(height - 1);
			}
			break;
		case 4:
			if (getx() <= 1) {
				move(5);
				setx(1);
			}
			break;
		}
	}

	void setChargephase(int num) {
		ChargePhase = num;
	}

	void setPowerBar(int num) {
		PowerBar = num;
	}

	void setPhaseStatus(bool stat) {
		PhaseStatus = stat;
	}

	int getChargePhase() {
		return ChargePhase;
	}

	int getPowerBar() {
		return PowerBar;
	}

	bool getPhaseStatus() {
		return PhaseStatus;
	}
};

class CEnemy : public CMovableObject {
public:
	CEnemy(int x = rand() % 16 + 2, int y = rand() % 10 + 2, char c = '#', int lx = rand() % 16 + 2, int ly = rand() % 10 + 2, int dir = rand() % 4 + 1) : CMovableObject(x, y, c, lx, ly, dir) {
		if (getx() == width / 2 || gety() == height / 2) {
			setx(rand() % 16 + 2); sety(rand() % 10 + 2);
		}
	}

	void updates() {
		switch (this->getdir()) {
		case 1:
			this->moveup();
			break;
		case 2:
			this->moveright();
			break;
		case 3:
			this->movedown();
			break;
		case 4:
			this->moveleft();
			break;
		}

		if (this->getx() == width) {
			this->setdir(4);
		}
		if (this->getx() == 0) {
			this->setdir(2);
		}
		if (this->gety() == 0) {
			this->setdir(3);
		}
		if (this->gety() == height) {
			this->setdir(1);
		}
	}
};

class CGamePacman {
	bool GameOver;
	CTimer timer;
	CFood foods[wincon];
	CPlayer player;
	CEnemy enemy[opponent];
public:
	void drawAbilityStatus() {
		gotoxy(1, 16);  cout << "Press 'C' to use Ability. Charge Phase = " << player.getChargePhase();
		gotoxy(1, 17);  cout << "Can use ability for " << player.getPowerBar() << " more times.";
		if (player.getPhaseStatus()) {
			gotoxy(1, 18);  cout << "Ability Active, Remaining Time (end if seconds >= 10.0): " << timer.elapsedSeconds();
		}
		else {
			gotoxy(1, 18); cout << "Ability Inactive";
		}
	}

	void drawfood() {
		for (int i = 0; i < wincon; i++) {
			foods[i].draw();
		}
	}

	void drawenemy() {
		for (int i = 0; i < opponent; i++) {
			enemy[i].draw();
		}
	}

	void updateenemy() {
		for (int i = 0; i < opponent; i++) {
			enemy[i].updates();
		}
	}

	void checkfood(CPlayer& player) {
		for (int i = 0; i < wincon; i++) {
			if (foods[i].getc() != ' ' && player.getx() == foods[i].getx() && player.gety() == foods[i].gety()) {
				player.setChargephase(player.getChargePhase() + 1); foods[i].set(foods[i].getx(), foods[i].gety(), ' ');
			}
		}
	}

	void checkenemy(CPlayer& player) {
		for (int i = 0; i < opponent; i++) {
			switch (enemy[i].getdir()) {
			case 1:
				if (player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() - 1 && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() + 1 && player.gety() == enemy[i].gety()) {
					GameOver = true;
				}
				break;
			case 2:
				if (player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() - 1 || player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() + 1) {
					GameOver = true;
				}
				break;
			case 3:
				if (player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() - 1 && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() + 1 && player.gety() == enemy[i].gety()) {
					GameOver = true;
				}
				break;
			case 4:
				if (player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() || player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() - 1 || player.getx() == enemy[i].getx() && player.gety() == enemy[i].gety() + 1) {
					GameOver = true;
				}
				break;
			}
		}
	}

	void AbilityActive() {
		if (player.getPowerBar() > 0) {
			if (player.getPhaseStatus() != true) {
				player.setPowerBar(player.getPowerBar() - 1);
				timer.start(); player.setPhaseStatus(true);
			}
		}
	}

	void AbiltyInactive() {
		if (timer.elapsedSeconds() >= 10.0) {
			timer.stop(); player.setPhaseStatus(false);
		}
	}

	void PowerIncrease() {
		if (player.getChargePhase() > 0 && player.getChargePhase() % 20 == 0)
			player.setPowerBar(player.getPowerBar() + 1);
	}

	void Initialize() {
		int j = 0;
		for (int i = 1; i < height; i++) {
			for (int q = 1; q < width; q++) {
				foods[j].set(q, i, '.'); j += 1;
			}
		}
		GameOver = false;
	}

	void Draw() {
		system("CLS");
		drawfood();
		player.draw();
		drawenemy();
		drawAbilityStatus();
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
			case 'c': // using ability for 10 seconds, press once
				AbilityActive();
				break;
			case 'z': // exit game
				GameOver = true;
				break;
			}
		}
	}

	void Updates() {
		AbiltyInactive();
		if (!player.getPhaseStatus()) {
			checkfood(player);
			checkenemy(player);
			PowerIncrease();
		}
		player.updates();
		updateenemy();
	}

	void Check() {
		if (player.getChargePhase() >= wincon) {
			GameOver = true;
		}
	}

	void End() {
		system("CLS");
		gotoxy(1, 1); cout << "Your Score: " << player.getChargePhase();
		if (player.getChargePhase() >= wincon) {
			gotoxy(1, 2); cout << "You Win!! " << endl;
		}
		else {
			gotoxy(1, 2); cout << "You Lose!! " << endl;
		}
	}

	void Run() {
		while (!GameOver) {
			Draw();
			ProcessInput();
			Sleep(10);
			Updates();
			Check();
		}
		End();
	}
};

void aturan() {
	system("CLS");
	cout << " ===== C++ Console Pacman Game ===== \n" << endl;
	cout << "Aturan permainan adalah sebagai berikut: " << endl;
	cout << "1. Permainan berakhir game over jika pemain dimakan (ditabrak) musuh. " << endl;
	cout << "   Pemain dianggap menabrak musuh jika posisinya sama atau bersebelahan. " << endl;
	cout << "2. Pemain menang jika berhasil memakan semua titik. " << endl;
	cout << "3. Kontrol pemain dengan tombol A, S, D, W. Z untuk exit game. X untuk stop." << endl;
	cout << "   dan C untuk aktifkan ability. " << endl;
	cout << "4. Di awal permainan, pemain tidak bergerak dan baru bergerak tombol bergerak ditekan. " << endl;
	cout << "5. Jika menyentuh dinding pembatas, pemain berhenti. " << endl;
	cout << "6. Dinding tidak terlihat untuk menambah kesusahan dari game ini. " << endl;
	Sleep(8000);
	system("CLS");
}

int main() {
	aturan();
	srand(time(NULL));
	CGamePacman game;
	game.Initialize();
	game.Run();
	return 0;
}