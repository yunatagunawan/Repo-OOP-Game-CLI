//  Space Impact 3.0 //
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <time.h>

using namespace std;
using namespace chrono;

const int height = 20;
const int width = 110;
const double heightf = 20.0;
const double widthf = 110.0;

void gotoxy(int x, int y) {
	COORD pos = { (short)x, (short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

class Timer {
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

		return static_cast<double>(duration_cast<milliseconds>(endTime - m_StartTime).count());
	}

	double elapsedSeconds() {
		return elapsedMilliseconds() / 1000.0;
	}
};

class Object {
protected:
	double x, y, dx, dy;
	int health;
public:
	Object() {}

	Object(double ix, double iy, double idx, double idy, int hp) : x(ix), y(iy), dx(idx), dy(idy), health(hp) {}

	double getx() { return x; }

	double gety() { return y; }

	double getdx() { return dx; }

	double getdy() { return dy; }

	int gethp() { return health; }

	void setx(double num) { x = num; }

	void sety(double num) { y = num; }

	void setdx(double num) { dx = num; }

	void setdy(double num) { dy = num; }

	void sethp(int num) { health = num; }

	virtual void setstatus(bool s) {}

	virtual void moveup() {
		x = dx; y = dy;
		dx = dx; dy = dy - 0.5;
	}

	virtual void moveright() {
		x = dx; y = dy;
		dx = dx + 0.5; dy = dy;
	}

	virtual void movedown() {
		x = dx; y = dy;
		dx = dx; dy = dy + 0.5;
	}

	virtual void moveleft() {
		x = dx; y = dy;
		dx = dx - 0.5; dy = dy;
	}

	virtual void display() = 0;

	virtual void Update() {}

	virtual bool getstatus() { return 0; }
};

class Bullet : public Object {
public:
	Bullet() {}

	Bullet(double ix, double iy, double idx, double idy, int dmg) : Object(ix, iy, idx, idy, dmg) {}

	void display() {
		gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << "-";
	}

	void moveright() {
		x = dx; y = dy;
		dx = dx + 1.0; dy = dy;
	}

	void Update() {
		moveright();
		if (dx >= width) {
			dx = static_cast<double>(width) - 1.0;
		}
	}
};

class Player : public Object {
public:
	Player() {}

	Player(double ix, double iy, double idx, double idy, int hp) : Object(ix, iy, idx, idy, hp) {}

	int gethp() { return health; }

	void sethp(int num) { health = num; }

	void display() {
		gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << " |\\==";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 1.0)); cout << "-| \\ ";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 2.0)); cout << "-|  )";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 3.0)); cout << "-| / ";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 4.0)); cout << " |/==";
	}

	void moveup() {
		x = dx; y = dy;
		dx = dx; dy = dy - 1.0;
	}

	void moveright() {
		x = dx; y = dy;
		dx = dx + 1.0; dy = dy;
	}

	void movedown() {
		x = dx; y = dy;
		dx = dx; dy = dy + 1.0;
	}

	void moveleft() {
		x = dx; y = dy;
		dx = dx - 1.0; dy = dy;
	}

	void Update() {

		if (dx <= 0) {
			dx = 1.0;
		}
		if (dx + 4 >= width) {
			dx = static_cast<double>(width) - 5.0;
		}
		if (dy <= 0) {
			dy = 1.0;
		}
		if (dy + 4 >= height) {
			dy = static_cast<double>(height) - 5.0;
		}
	}
};

class Biasa : public Object {
public:
	Biasa() {}

	Biasa(double ix, double iy, double idx, double idy, int hp) : Object(ix, iy, idx, idy, hp) {}

	void display() {
		gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << "==== ";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 1.0)); cout << " * * ";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 2.0)); cout << "*   *";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 3.0)); cout << " * * ";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 4.0)); cout << "==== ";
		gotoxy(static_cast<int>(dx + 2.0), static_cast<int>(dy + 2.0)); cout << health;
	}

	void Update() {
		moveleft();
		if (dx <= 0) {
			dx = 1.0;
		}
	}
};

class Zigzag : public Object {
public:
	Zigzag() {}

	Zigzag(double ix, double iy, double idx, double idy, int hp) : Object(ix, iy, idx, idy, hp) {}

	void display() {
		gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << "-oooo";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 1.0)); cout << "o   o";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 2.0)); cout << "o   o";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 3.0)); cout << "o   o";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 4.0)); cout << "-oooo";
		gotoxy(static_cast<int>(dx + 2.0), static_cast<int>(dy + 2.0)); cout << health;
	}

	void Update() {
		moveleft();
		switch (rand() % 2 + 1) {
		case 1:
			moveup();
			break;
		case 2:
			movedown();
			break;
		}
		if (dx <= 0) {
			dx = 1.0;
		}
		if (dy <= 0) {
			dy = 1.0;
		}
		if (dy + 4 >= height) {
			dy = static_cast<double>(height) - 5.0;
		}
	}
};

class Seeker : public Object {
public:
	Seeker() {}

	Seeker(double ix, double iy, double idx, double idy, int hp) : Object(ix, iy, idx, idy, hp) {}

	void display() {
		gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << "-----";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 1.0)); cout << " /  |";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 2.0)); cout << "{   |";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 3.0)); cout << " \\  |";
		gotoxy(static_cast<int>(dx), static_cast<int>(dy + 4.0)); cout << "-----";
		gotoxy(static_cast<int>(dx + 2.0), static_cast<int>(dy + 2.0)); cout << health;
	}

	void moveup() {
		x = dx; y = dy;
		dx = dx; dy = dy - 1.0;
	}

	void movedown() {
		x = dx; y = dy;
		dx = dx; dy = dy + 1.0;
	}

	void moveleft() {
		x = dx; y = dy;
		dx = dx - 1.0; dy = dy;
	}

	void Update() {
		moveleft();
		if (dx <= 0) {
			dx = 1.0;
		}
	}
};

class Shield : public Object {
private:
	bool shieldactive;
public:
	Shield() {}

	Shield(double ix, double iy, double idx, double idy, int hp, bool f) : Object(ix, iy, idx, idy, hp) { shieldactive = f; }

	bool getstatus() { return shieldactive; }

	void setstatus(bool s) { shieldactive = s; }

	void display() {
		if (shieldactive) {
			gotoxy(static_cast<int>(dx - 1.0), static_cast<int>(dy)); cout << "#/===\\";
			gotoxy(static_cast<int>(dx - 1.0), static_cast<int>(dy + 1.0)); cout << "#|   |";
			gotoxy(static_cast<int>(dx - 1.0), static_cast<int>(dy + 2.0)); cout << "#|   |";
			gotoxy(static_cast<int>(dx - 1.0), static_cast<int>(dy + 3.0)); cout << "#|   |";
			gotoxy(static_cast<int>(dx - 1.0), static_cast<int>(dy + 4.0)); cout << "#\\===/";
			gotoxy(static_cast<int>(dx + 2.0), static_cast<int>(dy + 2.0)); cout << health;
		}
		else {
			gotoxy(static_cast<int>(dx), static_cast<int>(dy)); cout << "/===\\";
			gotoxy(static_cast<int>(dx), static_cast<int>(dy + 1.0)); cout << "|   |";
			gotoxy(static_cast<int>(dx), static_cast<int>(dy + 2.0)); cout << "|   |";
			gotoxy(static_cast<int>(dx), static_cast<int>(dy + 3.0)); cout << "|   |";
			gotoxy(static_cast<int>(dx), static_cast<int>(dy + 4.0)); cout << "\\===/";
			gotoxy(static_cast<int>(dx + 2.0), static_cast<int>(dy + 2.0)); cout << health;
		}
	}

	void Update() {
		moveleft();
		if (dx <= 0) {
			dx = 1.0;
		}
	}
};

class GameSpaceImpact {
private:
	Timer timer;
	Object* data[6];
	Object** bullets;
	bool over, enemyability;
	int score, bulletcount, dir;
	int hp;

	void Rules() {
		system("CLS");
		cout << "Rules: " << endl;
		cout << "1. Pesawat Biasa, bergerak normal dari kanan ke kiri \n2. Pesawat ZigZag, bergerak dari kanan ke kiri dengan pola zig - zag atas bawah \n";
		cout << "3. Pesawat Seeker, bergerak dari kanan ke kiri dengan lebih cepat dari pesawat lainnya \n";
		cout << "   dan posisi Y nya selalu mendekat ke posisi Y pemain. \n3. Pesawat Shield, bergerak normal dari kanan ke kiri, tetapi setiap beberapa saat \n   memasuki mode Shield untuk selang waktu tertentu.";
		cout << " Dalam mode shield pesawat ini tidak bisa bergerak,\n   namun kebal dari peluru, \n   ada juga tampilan perisai di depan pesawat ketika mode Shield aktif.";
		cout << "\n5. Player menang apabila sudah mengalahkan 25 musuh\n";
		cout << "6. Player kalah apabila musuh menabrak player atau 5 pesawat musuh mencapai garis kiri\n";
		Sleep(8000);
	}

	void control() {
		gotoxy(1, 23); cout << "Bullet Count: " << bulletcount;
		gotoxy(1, 24); cout << "Press AWSD to move, Z to stop, L to fire bullet, X to quit game.";
		gotoxy(1, 25); cout << "*defeat 25 enemies: " << score;
		gotoxy(1, 26); cout << "*Batas Garis: " << hp;
	}

	void Win() {
		if (score >= 25 && hp > 0) {
			over = true;
		}
	}

	void SeekerAbilities() {
		data[3]->Update();
		if (data[3]->getdy() < data[0]->getdy()) {
			data[3]->setdy(data[3]->getdy() + 0.25);
		}
		else if (data[3]->getdy() > data[0]->getdy()) {
			data[3]->setdy(data[3]->getdy() - 0.25);
		}
	}

	void ShieldAbilities() {
		if (data[4]->getstatus()) {
			if (timer.elapsedSeconds() >= 3.0) {
				timer.stop();
				timer.start();
				data[4]->setstatus(false);
				enemyability = true;
			}
		}
		else if (enemyability) {
			if (timer.elapsedSeconds() < 5.0) {
				data[4]->Update();
			}
			else {
				timer.stop();
				timer.start();
				data[4]->setstatus(true);
				enemyability = false;
			}
		}
	}

	void ShootBullet() {
		Object** temp = new Object * [bulletcount + 1];
		for (int i = 0; i < bulletcount + 1; i++) {
			if (i >= bulletcount) {
				temp[i] = new Bullet(data[0]->getx() + 5, data[0]->gety() + 2, data[0]->getdx() + 5, data[0]->getdy() + 2, 1);
			}
			else {
				temp[i] = new Bullet(bullets[i]->getx(), bullets[i]->gety(), bullets[i]->getdx(), bullets[i]->getdy(), 1);
			}
		}
		for (int i = 0; i < bulletcount; i++) {
			delete[]bullets[i];
		}
		delete[]bullets;
		bullets = new Object * [bulletcount + 1];
		for (int i = 0; i < bulletcount + 1; i++) {
			bullets[i] = new Bullet(temp[i]->getx(), temp[i]->gety(), temp[i]->getdx(), temp[i]->getdy(), 1);
		}
		for (int i = 0; i < bulletcount + 1; i++) {
			delete[]temp[i];
		}
		delete[]temp;
		bulletcount += 1;
	}

	void CheckAllBulletsHitWall() {
		if (bulletcount > 0) {
			for (int i = 0; i < bulletcount; i++) {
				if (bullets[i]->getdx() >= widthf - 1 || bullets[i]->getx() >= widthf - 1) {
					Bullethitwall(i);
				}
			}
		}
	}

	void CheckAllBulletsHitEnemy() {
		if (bulletcount > 0) {
			for (int i = 1; i < 6; i++) {
				for (int j = 0; j < bulletcount; j++) {
					if (bullets[j]->getdx() >= data[i]->getdx() && bullets[j]->getdx() <= data[i]->getdx() + 4 && bullets[j]->getdy() >= data[i]->getdy() && bullets[j]->getdy() <= data[i]->getdy() + 4) {
						if (i != 4) {
							Bullethitenemy(i);
							data[i]->sethp(data[i]->gethp() - 1);
						}
						else if (i == 4 && data[4]->getstatus()) {} // shield active so 
						else {
							Bullethitenemy(i);
							data[i]->sethp(data[i]->gethp() - 1);
						}
					}
				}
			}
		}
	}

	void Bullethitwall(int index) {
		if (bulletcount > 1) {
			swap(bullets[index], bullets[bulletcount - 1]);
		}
		Object** temp = new Object * [bulletcount - 1];
		for (int i = 0; i < bulletcount - 1; i++) {
			temp[i] = new Bullet(bullets[i]->getx(), bullets[i]->gety(), bullets[i]->getdx(), bullets[i]->getdy(), 1);
		}
		for (int i = 0; i < bulletcount; i++) {
			delete[]bullets[i];
		}
		delete[]bullets;
		bullets = new Object * [bulletcount - 1];
		for (int i = 0; i < bulletcount - 1; i++) {
			bullets[i] = new Bullet(temp[i]->getx(), temp[i]->gety(), temp[i]->getdx(), temp[i]->getdy(), 1);
		}
		for (int i = 0; i < bulletcount - 1; i++) {
			delete[]temp[i];
		}
		delete[]temp;
		bulletcount -= 1;
	}

	void Bullethitenemy(int index) {
		if (bulletcount > 1) {
			for (int j = 0; j < bulletcount - 1; j++) {
				if (bullets[j]->getdx() >= data[index]->getdx() && bullets[j]->getdx() <= data[index]->getdx() + 4 && bullets[j]->getdy() >= data[index]->getdy() && bullets[j]->getdy() <= data[index]->getdy() + 4) {
					swap(bullets[j], bullets[bulletcount - 1]); break;
				}
			}
		}
		Object** temp = new Object * [bulletcount - 1];
		for (int i = 0; i < bulletcount - 1; i++) {
			temp[i] = new Bullet(bullets[i]->getx(), bullets[i]->gety(), bullets[i]->getdx(), bullets[i]->getdy(), 1);
		}
		for (int i = 0; i < bulletcount; i++) {
			delete[]bullets[i];
		}
		delete[]bullets;
		bullets = new Object * [bulletcount - 1];
		for (int i = 0; i < bulletcount - 1; i++) {
			bullets[i] = new Bullet(temp[i]->getx(), temp[i]->gety(), temp[i]->getdx(), temp[i]->getdy(), 1);
		}
		for (int i = 0; i < bulletcount - 1; i++) {
			delete[]temp[i];
		}
		delete[]temp;
		bulletcount -= 1;
	}

	void BulletUpdate() {
		for (int i = 0; i < bulletcount; i++)
			bullets[i]->Update();
	}

	void BulletDraw() {
		for (int i = 0; i < bulletcount; i++)
			bullets[i]->display();
	}

	void EnemyReset() {
		for (int i = 1; i < 6; i++) {
			if (data[i]->gethp() <= 0) {
				data[i]->setdx(widthf - 5);
				data[i]->setdy(rand() % 15 + 1.0);
				data[i]->sethp(rand() % 6 + 3);
				score += 1;
			}
		}
	}

	void EnemyReachEnd() {
		for (int i = 1; i < 6; i++) {
			if (data[i]->getdx() <= 1) {
				hp -= 1;
				data[i]->setdx(widthf - 5);
				data[i]->setdy(rand() % 15 + 1.0);
				data[i]->sethp(rand() % 6 + 3);
			}
		}
	}

	void EnemyCollisionDetect() {
		if (hp <= 0) { over = true; }
		for (int i = 1; i < 6; i++) {
			if (data[0]->getdx() >= data[i]->getdx() && data[0]->getdx() <= data[i]->getdx() + 4 && data[0]->getdy() >= data[i]->getdy() && data[0]->getdy() <= data[i]->getdy() + 4) { over = true; }
			else if (data[0]->getdx() + 4 >= data[i]->getdx() && data[0]->getdx() + 4 <= data[i]->getdx() + 4 && data[0]->getdy() >= data[i]->getdy() && data[0]->getdy() <= data[i]->getdy() + 4) { over = true; }
			else if (data[0]->getdx() >= data[i]->getdx() && data[0]->getdx() <= data[i]->getdx() + 4 && data[0]->getdy() + 4 >= data[i]->getdy() && data[0]->getdy() + 4 <= data[i]->getdy() + 4) { over = true; }
			else if (data[0]->getdx() + 4 >= data[i]->getdx() && data[0]->getdx() + 4 <= data[i]->getdx() + 4 && data[0]->getdy() + 4 >= data[i]->getdy() && data[0]->getdy() + 4 <= data[i]->getdy() + 4) { over = true; }
		}
	}

	void map() {
		for (int i = 0; i <= height; i++) {
			for (int j = 0; j <= width; j++) {
				if (i == 0 || i == height) {
					gotoxy(j, i); cout << "-";
				}
				else if (j == 0 || j == width) {
					gotoxy(j, i); cout << "|";
				}
			}
		}
	}
public:
	void Initialize() {
		over = false;
		enemyability = true;
		score = 0;
		bulletcount = 0;
		hp = 5;
		data[0] = new Player(1, 8, 1, 8, 1);
		data[1] = new Biasa(widthf - 5, 1, widthf - 5, 1, rand() % 6 + 3);
		data[2] = new Zigzag(widthf - 15, 8, widthf - 15, 8, rand() % 6 + 3);
		data[3] = new Seeker(widthf - 5, 8, widthf - 5, 8, rand() % 6 + 3);
		data[4] = new Shield(widthf - 5, 15, widthf - 5, 15, rand() % 6 + 3, false);
		data[5] = new Biasa(widthf + 10, 1, widthf + 10, rand() % 15 + 1.0, rand() % 6 + 3);
		timer.start();
	}

	void Draw() {
		system("CLS");
		data[0]->display();
		data[1]->display();
		data[2]->display();
		data[3]->display();
		data[4]->display();
		data[5]->display();
		BulletDraw();
		map();
		EnemyCollisionDetect();
		EnemyReachEnd();
		control();
	}

	void ProcessInput() {
		if (_kbhit()) {
			switch (_getch())
			{
			case 'a':
				dir = 4;
				break;
			case 'w':
				dir = 1;
				break;
			case 's':
				dir = 3;
				break;
			case 'd':
				dir = 2;
				break;
			case 'z':
				dir = 5;
				break;
			case 'l':
				ShootBullet();
				break;
			case 'x':
				over = true;
				break;

			case 'p':
				Sleep(30000);
				break;
			}

		}
		switch (dir) {
		case 1:
			data[0]->moveup();
			break;
		case 2:
			data[0]->moveright();
			break;
		case 3:
			data[0]->movedown();
			break;
		case 4:
			data[0]->moveleft();
			break;
		case 5:
			break;
		}
	}

	void Update() {
		data[0]->Update();
		data[1]->Update();
		data[2]->Update();
		data[5]->Update();
		SeekerAbilities();
		ShieldAbilities();
		EnemyReset();
		BulletUpdate();
		CheckAllBulletsHitWall();
		CheckAllBulletsHitEnemy();
	}

	void End() {
		timer.stop();
		for (int i = 0; i < bulletcount; i++) {
			delete[]bullets[i];
		}
		delete[]bullets;

		if (score >= 25) {
			system("CLS");
			gotoxy(1, 1); cout << "You Win!";
		}
		else {
			system("CLS");
			gotoxy(1, 1); cout << "You Lose!";
		}
	}

	void Run() {
		Initialize();
		Rules();
		while (!over) {
			Draw();
			ProcessInput();
			Sleep(100);
			Update();
			Win();
		}
		End();
	}
};

int main() {
	srand(time(NULL));
	GameSpaceImpact game;
	game.Run();
	return 0;
}