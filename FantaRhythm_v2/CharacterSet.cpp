#include"CharacterSet.h"
#include "Soldier.h"

CharacterSet::CharacterSet(int save[]) {
	CSVData csv;
	csv.load(U"resources/charadata.csv");
	
	for (int i = 0; i < CHANUMBER; i++) {
		int initx = 900 + i * 90, inity = 150 + i * 80;//初期座標の設定後で見直す
		switch (save[i]) {
		case JOB::SOLDIER:
			cha[i] = new Soldier(csv, initx, inity, i);
			break;
		default:
			//エラー
			break;
		}
	}

}

CharacterSet::~CharacterSet() {
	for (int i = 0; i < CHANUMBER; i++) {
		delete cha[i];
	}
}

void CharacterSet::draw() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->draw();
	}
}

void CharacterSet::update() {
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->update();
	}
}

void CharacterSet::funcEvent(Obj obj) {//イベントを通達
	cha[obj.lane]->getEvent(obj.msg);
	
}