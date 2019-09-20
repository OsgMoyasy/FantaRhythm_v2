#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//賢者のジョブ
//上ボタンで回復量のチャージ
//下ボタンである一定のチャージ量があれば回復

Sage::Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"sage", csv, ix, iy, row) {
	recoveryClear();
}

Sage::~Sage() {

}

void Sage::draw() {
	Character::chaDraw();
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

int Sage::heal() {
	if (recoverycount > 5) {
		curehp = getPower() * recoverycount;
		return curehp;
	}
	return 0;
}


void Sage::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		recoverycharge();
		setAttackEvent(getPower(), EffectType::NOMAL);
		break;
	case Massage::DOWNATTACK:
		heal();
		recoveryClear();
		break;
	}
}
