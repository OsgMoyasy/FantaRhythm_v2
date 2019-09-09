#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//���҂̃W���u
//��{�^���ŉ񕜗ʂ̃`���[�W
//���{�^���ł�����̃`���[�W�ʂ�����Ή�

Sage::Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"sage", csv, ix, iy, row) {
	recoverycount = 0;
}

Sage::~Sage() {

}

void Sage::draw() {
	Character::chaDraw();
}

void Sage::update() {
	moveUpDown();
	Print << U"recovery count=" << recoverycount;			//�����Ă����̐��A���O�͉�
}

void Sage::recoverycharge() {
	if (recoverycount < RECOVERYMAX) {
		recoverycount += 1;
	}
}


void Sage::Qrecovery() {
	if (recoverycount > 5) {
		curehp = getPower() * recoverycount;
	}

}

void Sage::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		recoverycharge();
		setAttackEvent(getPower(), EffectType::NOMAL);
		break;
	case Massage::DOWNATTACK:
		Qrecovery();
		break;
	}
}

