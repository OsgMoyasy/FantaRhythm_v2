#include "Combostar.h"
constexpr int COMBOMAX = 30;

Combostar::Combostar(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"combostar", csv, ix, iy, row) {
	comboClear();
	combodamage = 0;
	updamage = 0;
}

Combostar::~Combostar() {

}

void Combostar::jobDraw() {

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
}
void Combostar::downEvent(void) {
	comboAttack();
}
void Combostar::damageEvent(void) {
	comboClear();
}

