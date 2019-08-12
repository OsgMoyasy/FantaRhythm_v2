#include"CharacterSet.h"
#include "Soldier.h"

CharacterSet::CharacterSet(int save[]) {
	CSVData csv;
	csv.load(U"resources/charadata.csv");
	
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i] = new Soldier(csv,900 + i * 90, 150 + i * 80,i);
	}

}

CharacterSet::~CharacterSet() {

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