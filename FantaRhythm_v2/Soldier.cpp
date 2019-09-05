#include "Soldier.h"
#define CHARGEMAX 10

Soldier::Soldier(const CSVData& csv, double ix, double iy, int row, CharacterSubject* csubject) :Character(csv, ix, iy, row, csubject, U"sol") {
	chargeClear();
	chargedamage = 0;
}

Soldier::~Soldier() {

}

void Soldier::draw() {
	Character::chaDraw();
	drawEffect();
}

void Soldier::update() {
	moveUpDown();
	Print << U"charge=" << chargecount;
}

void Soldier::charge() {
	if (chargecount < CHARGEMAX) {
		chargecount +=1;
	}
}

void Soldier::chargeClear() {
	chargecount = 1;
}

void Soldier::chargeAttack() {
	chargedamage=getPower()* (std::pow(getArgs1(), chargecount / CHARGEMAX * 10));
}

void Soldier::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		charge();
		setAttackEvent(getPower());
		break;
	case Massage::DOWNATTACK:
		chargeAttack();
		break;
		chargeClear();
		break;
	}
}

