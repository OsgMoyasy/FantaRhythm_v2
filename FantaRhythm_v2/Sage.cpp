#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//賢者のジョブ
//上ボタンで回復量のチャージ
//下ボタンである一定のチャージ量があれば回復

Sage::Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"sage", csv, ix, iy, row) {
	recoveryClear();
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, U"resources/images/effects/sage", RECOVERYMAX, Color(Palette::Black), Color(Palette::Burlywood));
	chargeGauge->update(recoverycount);
}

Sage::~Sage() {

}

void Sage::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Sage::jobUpdate() {
	Print << U"recovery count=" << recoverycount;			
}

void Sage::recoverycharge() {
	if (recoverycount < RECOVERYMAX) {
		recoverycount += 1;
	}
}

void Sage::recoveryClear() {
	recoverycount = 0;
}

int Sage::isHeal() {
	if (recoverycount > 5) {
		curehp = getPower() * recoverycount;
		return curehp;
	}
	return 0;
}

void Sage::upEvent(void) {
	recoverycharge();
	setAttackEvent(getPower(), EffectType::NOMAL);
	chargeGauge->update(recoverycount);
}
void Sage::downEvent(void) {
	isHeal();
	recoveryClear();
	chargeGauge->update(recoverycount);
}
void Sage::damageEvent(void) {
	chargeGauge->update(recoverycount);
}
