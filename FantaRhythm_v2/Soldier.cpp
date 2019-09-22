#include "Soldier.h"
constexpr int CHARGEMAX = 10;

Soldier::Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"soldier", csv, ix, iy, row ) {
	chargeClear();
	chargedamage = 0;
}

Soldier::~Soldier() {

}

void Soldier::draw() {
	Character::characterDraw();
	drawEffect();
}

void Soldier::update() {
	moveUpDown();
	Print << U"charge=" << chargecount;
}

void Soldier::charge() {//小ダメージ　＆　チャージ
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (chargecount < CHARGEMAX) {
		chargecount +=1;
	}
}

void Soldier::chargeClear() {
	chargecount = 1;
}

void Soldier::chargeAttack() {
	chargedamage=getPower()* (std::pow(getArgs1(), chargecount / CHARGEMAX * 10));
	setAttackEvent(chargedamage, EffectType::ULT);
	chargeClear();
}

void Soldier::upEvent(void) {
	charge();
}
void Soldier::downEvent(void) {
	chargeAttack();
}
void Soldier::damageEvent(void) {
	chargeClear();
}
