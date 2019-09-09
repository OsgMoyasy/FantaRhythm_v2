#include "Sage.h"
constexpr int RECOVERYMAX = 15;

//賢者のジョブ
//上ボタンで回復量のチャージ
//下ボタンである一定のチャージ量があれば回復

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
	Print << U"recovery count=" << recoverycount;			//持っている矢の数、名前は仮
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

