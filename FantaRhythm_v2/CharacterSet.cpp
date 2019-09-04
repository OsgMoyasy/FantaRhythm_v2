#include"CharacterSet.h"
#include "Soldier.h"

CharacterSet::CharacterSet(int save[]) {
	CSVData csv;
	csv.load(U"resources/charadata.csv");
	totalhp = 0;
	starthp = 800;
	for (int i = 0; i < CHANUMBER; i++) {
		int initx = 900 + i * 90, inity = 150 + i * 80;//初期座標の設定後で見直す
		
		switch (csv.get<int>(save[i], 1)) {//キャラ番号の行のジョブを取得
		case JOB::SOLDIER:
			cha[i] = new Soldier(csv, initx, inity, i);
			break;
		default:
			//エラー
			break;
		}
		
		totalhp += csv.get<int>(save[i], 4);
	}
	if (totalhp >= 400) {
		totalhp = 400;
	}
	fixedhp = totalhp;
	damage = totalhp / 100;

}

CharacterSet::~CharacterSet() {
	for (int i = 0; i < CHANUMBER; i++) {
		delete cha[i];
	}
}

void CharacterSet::draw() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->draw();
		TotalhpDraw();
	}
}

void CharacterSet::update() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->update();

	}
}

void CharacterSet::funcEvent(Obj obj) {//イベントを通達
	cha[obj.lane]->getEvent(obj.msg);
	if (obj.msg == SMALLDAMAGE) {
		starthp += damage;
		totalhp -= damage;
	}
	
}

void CharacterSet::TotalhpDraw() {		//総HP表示
	Rect(800, 30, fixedhp, 40).draw(Palette::Red);
	Rect(starthp, 30, totalhp, 40).draw(Palette::Green);
}