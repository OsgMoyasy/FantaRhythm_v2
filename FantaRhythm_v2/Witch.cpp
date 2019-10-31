#include "Witch.h"
constexpr int CHARGEMAX = 10;

//仕様的にはSoldierと同じ
//魔女のジョブ

Witch::Witch(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) : Character(csubject, U"witch", char_name, hp, power, generic1, generic2, ix, iy) {
	chargeClear();
	chargedamage = 0;
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, U"resources/images/effects/witch", CHARGEMAX, Color(Palette::Black), Color(Palette::Orangered));
	chargeGauge->update(chargeCount);
}

Witch::~Witch() {

}

void Witch::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Witch::jobUpdate() {
	//Print << U"charge=" << chargeCount;
}

void Witch::charge() {				//Witchは基本チャージだけ
	aura->setFlag(true);
	if (chargeCount < CHARGEMAX) {
		chargeCount += 1;
	}
}

void Witch::chargeClear() {
	aura->setFlag(false);
	chargeCount = 1;
}

void Witch::chargeAttack() {
	chargedamage = (getPower()+getArgs1()) * chargeCount ;
	if (chargeCount > 8) {						//チャージのカウントが8回を超えた時
		chargedamage = ((getPower() + getArgs1()) * chargeCount )* getArgs2();
	}
	setAttackEvent(chargedamage, EffectType::NOMAL);
	chargeClear();
}

void Witch::upEvent(void) {
	charge();
	chargeGauge->update(chargeCount);
}
void Witch::downEvent(void) {
	chargeAttack();
	chargeGauge->update(chargeCount);
}
void Witch::damageEvent(void) {
	chargeClear();
	chargeGauge->update(chargeCount);
}

