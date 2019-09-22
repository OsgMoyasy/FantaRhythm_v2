#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//���҂̃W���u
//��{�^���ŉ񕜗ʂ̃`���[�W
//���{�^���ł�����̃`���[�W�ʂ�����Ή�

Sage::Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"sage", csv, ix, iy, row) {
	recoveryClear();
}

Sage::~Sage() {

}

void Sage::draw() {
	Character::characterDraw();
}

void Sage::update() {
	moveUpDown();
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
}
void Sage::downEvent(void) {
	isHeal();
	recoveryClear();
}
void Sage::damageEvent(void) {

}
