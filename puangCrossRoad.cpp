#include <iostream>
#include <bangtal>
#include <cstdio>
#include <time.h>
#include <Windows.h>


using namespace bangtal;
using namespace std;

class P {
public:
	int px;
	int py;
	P(int x, int y) :px(x), py(y) {}
};

class Car {
public:
	int x;
	int y;
	int speed;
	Car(int x, int y, int s) :x(x), y(y), speed(s) {}
};

int radius = 50;
int jump = 105;
TimerPtr change_timer;
ObjectPtr endButton;
ObjectPtr restartButton;
ScenePtr scene;
int stage = 0;
int earth_stage1 = 0, earth_stage2 = 0, earth_stage3 = 0;
static SoundPtr bgm;
static SoundPtr success;
static SoundPtr fail;
static SoundPtr e_sound;
static SoundPtr h_sound;
bool touch(int cx, int cy, int px, int py) {
	return((cx <= px + radius && cx >= px - radius && cy <= py + radius && cy >= py - radius));
}

//목숨 체크 함수
int heart = 3;
ObjectPtr heart1;
ObjectPtr heart2;
ObjectPtr heart3;
void heartCheck(int heart) {
	if (heart == 0) {
		fail->play();
		heart1->hide();
		stage = 0;
		earth_stage1 = 0;
		earth_stage2 = 0;
		earth_stage3 = 0;
		showMessage("여의주 찾아주기 실패 ㅜㅜ");
		endButton->locate(scene, 470, 610);
		endButton->setScale(0.5f);
		endButton->show();
		restartButton->show();
	}
	else if (heart == 1) {
		heart2->hide();
	}
	else if (heart == 2) {
		heart3->hide();
	}
	else {
		heart1->show();
		heart2->show();
		heart3->show();
	}
}
//여의주 체크 함수
int getNum = 0;
ObjectPtr getEarth1;
ObjectPtr getEarth2;
ObjectPtr getEarth3;
void earthCheck(int getNum) {

	if (getNum == 1) {
		getEarth1->show();
	}
	else if (getNum == 2) {
		getEarth2->show();
	}
	else if (getNum == 3) {
		getEarth3->show();
	}
	else {
		getEarth1->hide();
		getEarth2->hide();
		getEarth3->hide();
	}
}
//목표지점
void goalEnter(int getNum, int x, int y) {
	if (getNum == 3 && ((x >= 990) && (x <= 1010) && (y >= 600) && (y <= 620))) {
		showMessage("게임 성공!");
		success->play();
		stage = 0;
		earth_stage1 = 0, earth_stage2 = 0, earth_stage3 = 0;
		endButton->locate(scene, 470, 610);
		endButton->setScale(0.5f);
		endButton->show();
		restartButton->show();
	}
	else if (getNum < 3 && ((x >= 990) && (x <= 1010) && (y >= 600) && (y <= 620))) {
		showMessage("여의주를 더 모아야해!");
	}
}
//장애물 애니메이션 함수 right/left
int move_right(ScenePtr scene, ObjectPtr car, ObjectPtr puang, int cx, int cy, int cspeed, int px, int py) {
	if (cx < 1500 && touch(cx, cy, px, py) == false) {
		car->locate(scene, cx, cy);
		cx = cx + cspeed;
	}
	else if (touch(cx, cy, px, py)) {
		h_sound->play();
		Sleep(25);
		cx = cx + jump;
		puang->setImage("images/우는푸앙.png");
		car->locate(scene, cx, cy);
		change_timer->set(0.5);
		change_timer->start();
		heart = heart - 1;
	}
	else {
		car->locate(scene, 0, cy);
		cx = 0;
	}
	return cx;
}
int move_left(ScenePtr scene, ObjectPtr car, ObjectPtr puang, int cx, int cy, int cspeed, int px, int py) {
	if (cx >= -100 && touch(cx, cy, px, py) == false) {
		car->locate(scene, cx, cy);
		cx = cx - cspeed;
	}
	else if (touch(cx, cy, px, py)) {
		h_sound->play();
		Sleep(25);
		cx = cx - jump;
		puang->setImage("images/우는푸앙.png");
		car->locate(scene, cx, cy);
		change_timer->set(0.5);
		change_timer->start();
		heart = heart - 1;
	}
	else {
		car->locate(scene, 1280, cy);
		cx = 1280;
	}
	return cx;
}


int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	srand((unsigned)time(NULL));
	bgm = Sound::create("Monkeys Spinning Monkeys.mp3");
	success = Sound::create("jingles.mp3");
	fail = Sound::create("fail2.mp3");
	e_sound = Sound::create("Mario Jumping.mp3");
	h_sound = Sound::create("Metal Clang.mp3");
	auto scene0 = Scene::create("여의주 모으기", "images/첫화면.png");
	scene = Scene::create("여의주 모으기", "images/배경.png");
	auto goal = Object::create("images/학교.png", scene, 950, 610);
	goal->setScale(0.7f);

	auto startButton = Object::create("images/start.png", scene0, 430, 35);
	startButton->setScale(0.8f);
	restartButton = Object::create("images/restart.png", scene, 470, 650, false);
	restartButton->setScale(0.5f);
	endButton = Object::create("images/end.png", scene0, 630, 35);
	endButton->setScale(0.8f);
	//---모은 여의주
	getEarth1 = Object::create("images/0.png", scene, 50, 90, false);
	getEarth2 = Object::create("images/0.png", scene, 120, 90, false);
	getEarth3 = Object::create("images/0.png", scene, 190, 90, false);
	//---목숨
	heart1 = Object::create("images/heart.png", scene, 50, 20);
	heart1->setScale(0.15f);
	heart2 = Object::create("images/heart.png", scene, 120, 20);
	heart2->setScale(0.15f);
	heart3 = Object::create("images/heart.png", scene, 190, 20);
	heart3->setScale(0.15f);

	P puang_class(400, 130);
	auto puang = Object::create("images/푸앙.png", scene, puang_class.px, puang_class.py);

	//-----웃는/우는 표정 변화 후 다시 원래표정으로
	change_timer = Timer::create(1);
	change_timer->setOnTimerCallback([&](TimerPtr t)->bool {
		puang->setImage("images/푸앙.png");
		return 0;
		});

	////----------장애물1
	Car c1(-50, 250, 10);
	auto car1 = Object::create("images/1.png", scene, c1.x, c1.y);
	auto timer1 = Timer::create(0.1f);
	timer1->setOnTimerCallback([&](TimerPtr t)->bool {
		c1.x = move_right(scene, car1, puang, c1.x, c1.y, c1.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer1->set(0.1f);
			timer1->start();
			heartCheck(heart);
			earthCheck(getNum);
			goalEnter(getNum, puang_class.px, puang_class.py);
		}
		return 0;
		});
	////----------장애물2
	Car c2(1280, 350, 25);
	auto car2 = Object::create("images/7.png", scene, c2.x, c2.y);
	auto timer2 = Timer::create(0.1f);
	timer2->setOnTimerCallback([&](TimerPtr t)->bool {
		c2.x = move_left(scene, car2, puang, c2.x, c2.y, c2.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer2->set(0.1f);
			timer2->start();
		}
		return 0;
		});
	////////------장애물3
	Car c3(1200, 430, 15);
	auto car3 = Object::create("images/13.png", scene, c3.x, c3.y);
	auto timer3 = Timer::create(0.1f);
	timer3->setOnTimerCallback([&](TimerPtr t)->bool {
		c3.x = move_right(scene, car3, puang, c3.x, c3.y, c3.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer3->set(0.1f);
			timer3->start();
		}
		return 0;
		});
	//----------장애물4
	Car c4(0, 250, 20);
	auto car4 = Object::create("images/14.png", scene, c4.x, c4.y);
	auto timer4 = Timer::create(0.1f);
	timer4->setOnTimerCallback([&](TimerPtr t)->bool {
		c4.x = move_right(scene, car4, puang, c4.x, c4.y, c4.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer4->set(0.1f);
			timer4->start();
		}
		return 0;
		});
	//------------장애물5
	Car c5(1380, 530, 18);
	auto car5 = Object::create("images/5.png", scene, c5.x, c5.y);
	auto timer5 = Timer::create(0.1f);
	timer5->setOnTimerCallback([&](TimerPtr t)->bool {
		c5.x = move_left(scene, car5, puang, c5.x, c5.y, c5.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer5->set(0.1f);
			timer5->start();
		}
		return 0;
		});
	//------------장애물6
	Car c6(1380, 530, 12);
	auto car6 = Object::create("images/10.png", scene, c6.x, c6.y);
	auto timer6 = Timer::create(0.1f);
	timer6->setOnTimerCallback([&](TimerPtr t)->bool {
		c6.x = move_left(scene, car6, puang, c6.x, c6.y, c6.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer6->set(0.1f);
			timer6->start();
		}
		return 0;
		});
	//----------장애물7
	Car c7(1400, 430, 30);
	auto car7 = Object::create("images/8.png", scene, c7.x, c7.y);
	auto timer7 = Timer::create(0.1f);
	timer7->setOnTimerCallback([&](TimerPtr t)->bool {
		c7.x = move_right(scene, car7, puang, c7.x, c7.y, c7.speed, puang_class.px, puang_class.py);
		if (stage == 1) {
			timer7->set(0.1f);
			timer7->start();
		}
		return 0;
		});
	//-----여의주1
	Car e1(1280, 450, 25);
	auto earth1 = Object::create("images/0.png", scene, e1.x, e1.y);
	earth1->setScale(0.5f);
	auto t_earth1 = Timer::create(0.1f);
	t_earth1->setOnTimerCallback([&](TimerPtr t)->bool {
		if (e1.x >= 0 && touch(e1.x, e1.y, puang_class.px, puang_class.py) == false) {
			earth1->locate(scene, e1.x, e1.y);
			e1.x = e1.x - e1.speed;
		}
		else if (touch(e1.x, e1.y, puang_class.px, puang_class.py)) {
			e_sound->play();
			Sleep(25);
			e1.x = e1.x - 105;
			puang->setImage("images/웃는푸앙.png");
			earth1->hide();
			change_timer->set(0.5);
			change_timer->start();
			earth_stage1 = 0;
			getNum = getNum + 1;
		}
		else {
			earth1->locate(scene, 1280, e1.y);
			e1.x = 1280;
		}
		if (earth_stage1 == 1) {
			t_earth1->set(0.1f);
			t_earth1->start();

		}
		return 0;
		});
	//---여의주2
	Car e2(1280, 550, 15);
	auto earth2 = Object::create("images/0.png", scene, e2.x, e2.y);
	earth2->setScale(0.5f);
	auto t_earth2 = Timer::create(0.1f);
	t_earth2->setOnTimerCallback([&](TimerPtr t)->bool {
		if (e2.x >= 0 && touch(e2.x, e2.y, puang_class.px, puang_class.py) == false) {
			earth2->locate(scene, e2.x, e2.y);
			e2.x = e2.x - e2.speed;
		}
		else if (touch(e2.x, e2.y, puang_class.px, puang_class.py)) {
			e_sound->play();
			Sleep(25);
			e2.x = e2.x - 105;
			puang->setImage("images/웃는푸앙.png");
			earth2->hide();
			change_timer->set(0.5);
			change_timer->start();
			earth_stage2 = 0;
			getNum = getNum + 1;
		}
		else {
			earth2->locate(scene, 1280, e2.y);
			e2.x = 1280;
		}
		if (earth_stage2 == 1) {
			t_earth2->set(0.1f);
			t_earth2->start();
		}
		return 0;
		});
	//---여의주3
	Car e3(-100, 270, 35);
	auto earth3 = Object::create("images/0.png", scene, e3.x, e3.y);
	earth3->setScale(0.5f);
	auto t_earth3 = Timer::create(0.1f);
	t_earth3->setOnTimerCallback([&](TimerPtr t)->bool {
		if (e3.x <= 1300 && touch(e3.x, e3.y, puang_class.px, puang_class.py) == false) {
			earth3->locate(scene, e3.x, e3.y);
			e3.x = e3.x + e3.speed;
		}
		else if (touch(e3.x, e3.y, puang_class.px, puang_class.py)) {
			e_sound->play();
			Sleep(25);
			e3.x = e3.x + 105;
			puang->setImage("images/웃는푸앙.png");
			earth3->hide();
			change_timer->set(0.5);
			change_timer->start();
			earth_stage3 = 0;
			getNum = getNum + 1;
		}
		else {
			earth3->locate(scene, 0, e3.y);
			e3.x = 0;
		}
		if (earth_stage3 == 1) {
			t_earth3->set(0.1f);
			t_earth3->start();

		}
		return 0;
		});
	//---여의주 랜덤 딜레이 함수
	auto delay1 = Timer::create(rand() % 4 + 0.1f);
	delay1->setOnTimerCallback([&](TimerPtr t)->bool {
		t_earth1->start();
		return 0;
		});
	auto delay2 = Timer::create(rand() % 8 + 0.1f);
	delay2->setOnTimerCallback([&](TimerPtr t)->bool {
		t_earth2->start();
		return 0;
		});
	auto delay3 = Timer::create(rand() % 6 + 0.1f);
	delay3->setOnTimerCallback([&](TimerPtr t)->bool {
		t_earth3->start();
		return 0;
		});

	////----방향키
	scene->setOnKeyboardCallback([&](ScenePtr scene, int key, bool pressed)->bool {

		if ((puang_class.px >= 960 && puang_class.py <= 620 && key == 84)
			|| (puang_class.px < 960 && puang_class.py <= 500 && key == 84)) {
			puang_class.py = puang_class.py + 10;
			puang->locate(scene, puang_class.px, puang_class.py);
			cout << puang_class.py << endl;
			cout << puang_class.px << endl;
			return 0;
		}
		else if (puang_class.py >= 10 && key == 85)
		{
			puang_class.py = puang_class.py - 10;
			puang->locate(scene, puang_class.px, puang_class.py);
			cout << puang_class.py << endl;
			cout << puang_class.px << endl;
			return 0;
		}
		else if (puang_class.px >= 10 && key == 82)
		{
			puang_class.px = puang_class.px - 10;
			puang->locate(scene, puang_class.px, puang_class.py);
			cout << puang_class.py << endl;
			cout << puang_class.px << endl;
			return 0;
		}
		else if (puang_class.px <= 1200 && key == 83)
		{
			puang_class.px = puang_class.px + 10;
			puang->locate(scene, puang_class.px, puang_class.py);
			cout << puang_class.py << endl;
			cout << puang_class.px << endl;
			return 0;
		}
		});

	//----------------------
	startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		startButton->hide();
		endButton->hide();
		scene->enter();
		stage = 1;
		earth_stage1 = 1, earth_stage2 = 1, earth_stage3 = 1;
		timer1->start();
		timer2->start();
		timer3->start();
		timer4->start();
		timer5->start();
		timer6->start();
		timer7->start();
		delay1->start();
		delay2->start();
		delay3->start();
		bgm->play(true);
		return 0;
		});
	restartButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		restartButton->hide();
		startButton->hide();
		endButton->hide();
		earth1->show();
		earth2->show();
		earth3->show();
		heart = 3;
		getNum = 0;
		stage = 1;
		earth_stage1 = 1, earth_stage2 = 1, earth_stage3 = 1;
		puang_class.px = 400, puang_class.py = 130;
		puang->locate(scene, puang_class.px, puang_class.py);
		timer1->start();
		timer2->start();
		timer3->start();
		timer4->start();
		timer5->start();
		timer6->start();
		timer7->start();
		delay1->start();
		delay2->start();
		delay3->start();
		bgm->play(true);
		return 0;
		});
	endButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endGame();
		return 0;
		});

	startGame(scene0);
	return 0;
}