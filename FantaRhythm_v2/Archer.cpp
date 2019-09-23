#include "Archer.h"
constexpr int ARROWSMAX = 5;

//�|���̃W���u
//��{�^���ŋ|��~����(�ő�5�{)
//���{�^���Ŋm���U��

Archer::Archer(CharacterSubject* csubject, const CSVData & csv, double ix, double iy, int row) :Character(csubject, U"archer",csv, ix, iy, row ) {
	arrowscount = 0;
	arrowsdamage = 0;
}

Archer::~Archer() {

}

void Archer::jobDraw() {

}

void Archer::jobUpdate() {
	Print << U"Having arrows=" << arrowscount;			//�����Ă����̐��A���O�͉�
}

void Archer::arrowscharge() {		//��̃`���[�W(5�{�܂�)
	if (arrowscount < ARROWSMAX) {
		arrowscount += 1;
	}
}

void Archer::arrowsClear() {		//�U��������A����̐����Ȃ�Ό��炷
	if (arrowscount > 0) {
		arrowscount -= 1;
	}
}

void Archer::arrowsAttack() {
	if (arrowscount > 0) {			//��̐�������΃����_���Ń_���[�W�A�Ȃ���Ώ��1�_���[�W
		arrowsdamage = getPower() * Random(1, 20);
	}
	else {
		arrowsdamage = 1;
	}
	setAttackEvent(arrowsdamage, EffectType::ULT);
	arrowsClear();
}


void Archer::upEvent(void) {
	arrowscharge();
}
void Archer::downEvent(void) {
	arrowsAttack();
}
void Archer::damageEvent(void) {
	
}
