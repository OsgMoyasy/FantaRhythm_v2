#include "Archer.h"
constexpr int ARROWSMAX = 5;

//弓兵のジョブ
//上ボタンで弓を蓄える(最大5本)
//下ボタンで確率攻撃

Archer::Archer(CharacterSubject* csubject, const CSVData & csv, double ix, double iy, int row) :Character(csubject, U"archer",csv, ix, iy, row ) {
	arrowscount = 0;
	arrowsdamage = 0;
}

Archer::~Archer() {

}

void Archer::draw() {
	Character::chaDraw();
}

void Archer::update() {
	moveUpDown();
	Print << U"Having arrows=" << arrowscount;			//持っている矢の数、名前は仮
}

void Archer::arrowscharge() {
	if (arrowscount < ARROWSMAX) {
		arrowscount += 1;
	}
}

void Archer::arrowsClear() {
	if (arrowscount > 0) {
		arrowscount -= 1;
	}
}

void Archer::arrowsAttack() {
	arrowsdamage = getPower() * Random(1,20);
}


void Archer::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		arrowscharge();
		setAttackEvent(getPower(), EffectType::NOMAL);
		break;
	case Massage::DOWNATTACK:
		arrowsAttack();
		arrowsClear();
		break;
	}
}

