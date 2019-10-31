#include "Soldier.h"
constexpr int CHARGEMAX = 10;


Soldier::Soldier(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) :Character(csubject, U"soldier", char_name, hp, power, generic1, generic2, ix, iy){
	chargeClear();
	chargedamage = 0;
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, U"resources/images/effects/soldier", CHARGEMAX, Color(Palette::Black), Color(Palette::Blue));
	chargeGauge->update(chargeCount);
}

Soldier::~Soldier() {
	
}

void Soldier::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Soldier::jobUpdate() {
	//Print << U"charge=" << chargeCount;
}

void Soldier::charge() {//小ダメージ　＆　チャージ
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (chargeCount < CHARGEMAX) {
		chargeCount +=1;
	}
	else {
		aura->setFlag(true);
	}
}

void Soldier::chargeClear() {
	aura->setFlag(false);
	chargeCount = 10;
}

void Soldier::chargeAttack() {
	if (chargeCount > 8) {
		chargedamage =( getArgs1() * chargeCount ) * getArgs2();
	}
	else {
		chargedamage = getArgs1() * chargeCount;
	}
	setAttackEvent(chargedamage, EffectType::ULT);
	chargeClear();
}

void Soldier::upEvent(void) {
	charge();
	chargeGauge->update(chargeCount);
}
void Soldier::downEvent(void) {
	chargeAttack();
	chargeGauge->update(chargeCount);
}
void Soldier::damageEvent(void) {
	chargeClear();
	chargeGauge->update(chargeCount);
}