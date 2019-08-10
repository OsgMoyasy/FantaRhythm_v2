#include"CharacterSet.h"

CharacterSet::CharacterSet(int save[]) {

	CSVData csv;

	csv.load(U"resources/charadata.csv");
	for (int i = 0; i < CHANUMBER; i++) {
		cha[i] = new Character(900 + i * 50, 150 + i * 50);
	}
	/*
	for (int j = 0; j < csv.rows(); j++) {
		int lane = csv.get<int>(j, 2);

	}
	*/
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