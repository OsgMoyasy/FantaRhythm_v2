#include "Witch.h"
constexpr int CHARGEMAX = 10;

//�d�l�I�ɂ�Soldier�Ɠ���
//�����̃W���u

Witch::Witch(CharacterSubject* csubject, const CSVData & csv, double ix, double iy, int row) : Character(csubject, U"witch", csv, ix, iy, row) {
	chargeClear();
	chargedamage = 0;
}

Witch::~Witch() {

}

void Witch::draw() {
	Character::characterDraw();
}

void Witch::update() {
	moveUpDown();
	Print << U"charge=" << chargecount;
}

void Witch::charge() {				//Witch�͊�{�`���[�W����
	if (chargecount < CHARGEMAX) {
		chargecount += 1;
	}
}

void Witch::chargeClear() {
	chargecount = 1;
}

void Witch::chargeAttack() {
	chargedamage = (getPower()+getArgs1()) * chargecount ;
	if (chargecount > 8) {						//�`���[�W�̃J�E���g��8��𒴂�����
		chargedamage = ((getPower() + getArgs1()) * chargecount )* getArgs2();
	}
	setAttackEvent(chargedamage, EffectType::NOMAL);
	chargeClear();
}

void Witch::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		charge();
		break;
	case Massage::DOWNATTACK:
		chargeAttack();
		break;
	case Massage::DAMAGE:
		chargeClear();
		break;
	}
}

