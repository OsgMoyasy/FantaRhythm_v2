#include "Berserker.h"
constexpr int CHARGEMAX = 10;

Berserker::Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"Berserker", csv, ix, iy, row) {
	attackdamage = 0;		//���̓�͂��Ԃ�g��Ȃ�
	busterdamage = 0;	
	mydamage = 4;			//���{�^���U���ŐH�炤damage��
}

Berserker::~Berserker() {

}

void Berserker::draw() {
	Character::characterDraw();
	drawEffect();
}

void Berserker::update() {
	moveUpDown();
	//Print << U"charge=" << chargecount;
}

void Berserker::attack() {		//���ʂ̍U��
	attackdamage = getPower() + getArgs1();
	setAttackEvent(attackdamage, EffectType::ULT);
}


void Berserker::BusterAttack() {
	damage(mydamage);
	busterdamage = getPower() * getArgs1();
	setAttackEvent(busterdamage, EffectType::ULT);
}


void Berserker::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		attack();
		break;
	case Massage::DOWNATTACK:
		BusterAttack();
		break;
	}
}

