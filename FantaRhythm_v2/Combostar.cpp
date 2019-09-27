#include "Combostar.h"
constexpr int COMBOMAX = 30;

Combostar::Combostar(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"combostar", csv, ix, iy, row) {
	comboClear();
	combodamage = 0;
	updamage = 0;
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, getW(), 20, COMBOMAX, Color(Palette::Black), Color(Palette::Yellow));
	chargeGauge->update(combocount);
}

Combostar::~Combostar() {

}

void Combostar::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}

void Combostar::jobUpdate() {
	Print << U"combo=" << combocount;
}

void Combostar::combocharge() {		//小ダメージ　＆　コンボ加算
	updamage = getPower() + getArgs1();
	setAttackEvent(updamage, EffectType::NOMAL);
	if (combocount < COMBOMAX) {
		combocount += 1;
	}
}

void Combostar::comboClear() {
	combocount = 1;
}

void Combostar::comboAttack() {
	combodamage = (getPower() * combocount);
	if (combocount > 20) {
		combodamage *= getArgs2();
	}
	setAttackEvent(combodamage, EffectType::ULT);
}


void Combostar::upEvent(void) {
	combocharge();
	chargeGauge->update(combocount);
}
void Combostar::downEvent(void) {
	comboAttack();
	chargeGauge->update(combocount);
}
void Combostar::damageEvent(void) {
	comboClear();
	chargeGauge->update(combocount);
}

