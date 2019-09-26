#include "Berserker.h"
constexpr int CHARGEMAX = 10;

Berserker::Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"Berserker", csv, ix, iy, row) {
	attackdamage = 0;		//この二つはたぶん使わない
	busterdamage = 0;	
	selfcount = 0;			//downattackの回数
	mydamage = 4;			//下ボタン攻撃で食らうdamage量
}

Berserker::~Berserker() {

}

void Berserker::jobDraw() {
}


void Berserker::jobUpdate() {
	moveUpDown();
	Print << U"Berserker=" << attackdamage;
}

void Berserker::attack() {		//普通の攻撃
	attackdamage = getPower() + getArgs1();
	setAttackEvent(attackdamage, EffectType::ULT);
}


void Berserker::BusterAttack() {
	damage(mydamage);
	selfcount += 1;
	busterdamage = getPower() * ( getArgs1() / 10);
	if (selfcount > 8) {
		busterdamage *= getArgs2();
	}
	setAttackEvent(busterdamage, EffectType::ULT);
}

void Berserker::upEvent(void) {
	attack();
}
void Berserker::downEvent(void) {
	BusterAttack();
}
void Berserker::damageEvent(void) {

}

