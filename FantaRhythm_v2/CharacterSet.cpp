#include"CharacterSet.h"
#include "Soldier.h"

CharacterSet::CharacterSet(int save[]) {
	CSVData csv;
	csv.load(U"resources/charadata.csv");
	
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i] = new Soldier(csv,900 + i * 50, 150 + i * 50,0);
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
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i]->getEvent(obj);
	}
}