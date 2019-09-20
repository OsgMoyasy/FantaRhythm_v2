#include "Combostar.h"
constexpr int COMBOMAX = 30;

Combostar::Combostar(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"combostar", csv, ix, iy, row) {
	comboClear();
	combodamage = 0;
}

Combostar::~Combostar() {

}

void Combostar::draw() {
	Character::characterDraw();
	drawEffect();
}

void Combostar::update() {
	moveUpDown();
	Print << U"combo=" << combocount;
}

void Combostar::combocharge() {		//小ダメージ　＆　コンボ加算
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (combocount < COMBOMAX) {
		combocount += 1;
	}
}

void Combostar::comboClear() {
	combocount = 0;
}

void Combostar::comboAttack() {
	combodamage = getPower() * (std::pow(getArgs1(), combocount / 10));
	setAttackEvent(combodamage, EffectType::ULT);
}

void Combostar::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		combocharge();
		break;
	case Massage::DOWNATTACK:
		comboAttack();
		break;
	case Massage::DAMAGE:
		comboClear();
		break;
	}
}

