#include "Soldier.h"
constexpr int CHARGEMAX = 10;


Soldier::Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"soldier", csv, ix, iy, row ){
	chargeClear();
	chargedamage = 0;
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, getW(), 20, CHARGEMAX, Color(Palette::Black), Color(Palette::Yellow));
	chargeGauge->update(chargeCount);
}

Soldier::~Soldier() {
	
}

void Soldier::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Soldier::jobUpdate() {
	Print << U"charge=" << chargeCount;
}

void Soldier::charge() {//小ダメージ　＆　チャージ
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (chargeCount < CHARGEMAX) {
		chargeCount +=1;
	}
}

void Soldier::chargeClear() {
	chargeCount = 1;
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