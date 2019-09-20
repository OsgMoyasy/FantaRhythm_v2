#include "Berserker.h"
constexpr int CHARGEMAX = 10;

Berserker::Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"Berserker", csv, ix, iy, row) {
	attackdamage = 0;		//この二つはたぶん使わない
	busterdamage = 0;	
	mydamage = 4;			//下ボタン攻撃で食らうdamage量
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

void Berserker::attack() {		//普通の攻撃
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

