#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//賢者のジョブ
//上ボタンで回復量のチャージ
//下ボタンである一定のチャージ量があれば回復

Sage::Sage(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) :Character(csubject, U"sage", char_name, hp, power, generic1, generic2, ix, iy) {
	recoveryClear();
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, U"resources/images/effects/sage", RECOVERYMAX, Color(Palette::Black), Color(Palette::Burlywood));
	chargeGauge->update(recoverycount);
	canHeal = false;
	curehp = 0;
}

Sage::~Sage() {

}

void Sage::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Sage::jobUpdate() {
	//Print << U"recovery count=" << recoverycount;			
}

void Sage::recoverycharge() {
	if (recoverycount < RECOVERYMAX) {
		recoverycount += 1;
	}
	else {
		aura->setFlag(true);
	}
}

void Sage::recoveryClear() {
	recoverycount = 0;
}

int Sage::isHeal() {
	if (canHeal) {
		canHeal = false;
		if (recoverycount > 5) {
			aura->setFlag(false);
			curehp = getPower() * recoverycount;
			recoveryClear();
			chargeGauge->update(recoverycount);
			return curehp;
		}		
	}
	return 0;
}

void Sage::upEvent(void) {
	recoverycharge();
	setAttackEvent(getPower(), EffectType::NOMAL);
	chargeGauge->update(recoverycount);
}
void Sage::downEvent(void) {
	canHeal = true;
	playEffect(EffectType::ULT, getX(), getY());
}
void Sage::damageEvent(void) {
	chargeGauge->update(recoverycount);
}
