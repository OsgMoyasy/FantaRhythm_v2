#include "Soldier.h"

Soldier::Soldier(CSVData &csv, double ix, double iy, int row) :Character(csv, ix, iy, row) {
	
}

Soldier::~Soldier() {

}

void Soldier::draw() {
	Character::chaDraw();
}

void Soldier::update() {
	moveUpDown();
}

void Soldier::getEvent() {

}