#include"CharacterSet.h"
#include "Soldier.h"

constexpr int HPWIDTH = 400;
constexpr int HPHEIGHT = 30;

constexpr int HPLEFTX = 800;
constexpr int HPY = 30;

CharacterSet::CharacterSet(int save[], const String& musicpath) {
	csubject = new CharacterSubject();
	
	enemy = new Enemy(musicpath);
	csubject->addObserver(enemy);

	CSVData csv;
	csv.load(U"resources/charadata.csv");
	totalhp = 0;
	for (int i = 0; i < CHANUMBER; i++) {
		int initx = 900 + i * 90, inity = 250 + i * 80;//初期座標の設定後で見直す
		
		switch (csv.get<int>(save[i], 1)) {//キャラ番号の行のジョブを取得
		case JOB::SOLDIER:
			cha[i] = new Soldier(csubject, csv, initx, inity, i);
			break;
		default:
			//エラー
			break;
		}
		totalhp += csv.get<int>(save[i], 4);
	}
	currenthp = totalhp;
	calchpx();
	damage = 20;
}

CharacterSet::~CharacterSet() {
	for (int i = 0; i < CHANUMBER; i++) {
		delete cha[i];
	}
}

void CharacterSet::update() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->update();
	}
	enemy->update();
}

void CharacterSet::draw() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->draw();
		TotalhpDraw();
	}
	enemy->draw();
}



void CharacterSet::funcEvent(Obj obj) {//イベントを通達
	cha[obj.val]->getEvent(obj.msg);
	if (obj.msg == DAMAGE) {
		enemy->attack();
		selfDamage();
		
	}
	
}

void CharacterSet::TotalhpDraw() {		//総HP表示
	Rect(HPLEFTX, HPY, HPWIDTH, HPHEIGHT).draw(Palette::Red);
	Rect(HPLEFTX, HPY, hpx, HPHEIGHT).draw(Palette::Green);
}

int CharacterSet::getTotalDamage(void) {
	return enemy->getTotalDamage();
}

void CharacterSet::calchpx(void) {
	double per = (double)currenthp / totalhp;

	hpx = per * HPWIDTH;
}

void CharacterSet::selfDamage(void) {
	currenthp -= damage;
	if (currenthp > totalhp) {
		currenthp = totalhp;
	}
	else if (currenthp < 0) {
		currenthp = 0;
	}
	calchpx();
}

int CharacterSet::getCurrentHp(void) {
	return currenthp;
}

void CharacterSet::gameEndEffect(void) {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->damage();
	}
}