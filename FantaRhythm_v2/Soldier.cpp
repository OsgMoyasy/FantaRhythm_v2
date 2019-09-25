#include "Soldier.h"
constexpr int CHARGEMAX = 10;

Soldier::Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"soldier", csv, ix, iy, row ) {
	//chargeClear();
	chargecount = 9;
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
	if (chargecount > 8) {
		chargedamage =( getArgs1() * chargecount ) * getArgs2();
	}
	else {
		chargedamage = getArgs1() * chargecount;
	}

	setAttackEvent(chargedamage, EffectType::ULT);
	chargeClear();
}

void Soldier::getEvent(Massage msg) {
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

