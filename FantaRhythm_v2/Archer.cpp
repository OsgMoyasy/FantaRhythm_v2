#include "Archer.h"
#define ARROWSMAX 5

//�|���̃W���u
//��{�^���ŋ|��~����(�ő�5�{)
//���{�^���Ŋm���U��

Archer::Archer(CSVData & csv, double ix, double iy, int row, CharacterSubject * csubject) :Character(csv, ix, iy, row, csubject) {
	arrowsClear();
	arrowsdamage = 0;
}

Archer::~Archer() {

}

void Archer::draw() {
	Character::chaDraw();
}

void Archer::update() {
	moveUpDown();
	Print << U"Having arrows=" << arrowscount;			//�����Ă����̐��A���O�͉�
}

void Archer::arrowscharge() {
	if (arrowscount < ARROWSMAX) {
		arrowscount += 1;
	}
}

void Archer::arrowsClear() {
	chargecount = 0;
}

void Archer::chargeAttack() {
	chargedamage = getPower() * (std::pow(getArgs1(), chargecount / CHARGEMAX * 10));
}

void Archer::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		arrowscharge();
		setAttackEvent(getPower());
		break;
	case Massage::DOWNATTACK:
		arrowsAttack();
		break;
		arrowsClear();
		break;
	}
}

