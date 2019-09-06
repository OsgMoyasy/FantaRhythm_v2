#include "Witch.h"
#define CHARGEMAX 10

//�d�l�I�ɂ�Soldier�Ɠ���
//�����̃W���u

Witch::Witch(CharacterSubject* csubject, CSVData & csv, double ix, double iy, int row) : Character(csubject, U"witch", csv, ix, iy, row) {
	chargeClear();
	chargedamage = 0;
}

Witch::~Witch() {

}

void Witch::draw() {
	Character::chaDraw();
}

void Witch::update() {
	moveUpDown();
	Print << U"charge=" << chargecount;
}

void Witch::charge() {
	if (chargecount < CHARGEMAX) {
		chargecount += 1;
	}
}

void Witch::chargeClear() {
	chargecount = 1;
}

void Witch::chargeAttack() {
	chargedamage = getPower() * (std::pow(getArgs1(), chargecount / CHARGEMAX * 10));
}

void Witch::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		charge();
		setAttackEvent(getPower(),EffectType::NOMAL);
		break;
	case Massage::DOWNATTACK:
		chargeAttack();
		break;
		chargeClear();
		break;
	}
}
