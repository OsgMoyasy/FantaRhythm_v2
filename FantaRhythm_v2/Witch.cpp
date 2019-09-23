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

void Witch::jobDraw() {
}

void Witch::jobUpdate() {
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
	chargedamage = getPower() * (std::pow(getArgs1(), chargecount / CHARGEMAX * 10));
	setAttackEvent(chargedamage, EffectType::NOMAL);
	chargeClear();
}


void Witch::upEvent(void) {
	charge();
	setAttackEvent(getPower(), EffectType::NOMAL);
}
void Witch::downEvent(void) {
	chargeAttack();
}
void Witch::damageEvent(void) {

}

