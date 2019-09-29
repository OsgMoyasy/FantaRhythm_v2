#include "Berserker.h"
constexpr int CHARGEMAX = 5;

Berserker::Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"Berserker", csv, ix, iy, row) {
	attackdamage = 0;		//この二つはたぶん使わない
	busterdamage = 0;	
	upcount = 0;
	selfcount = 0;			//downattackの回数
	mydamage = 4;			//下ボタン攻撃で食らうdamage量
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, getW(), 20, CHARGEMAX, Color(Palette::Black), Color(Palette::Darkred));
	chargeGauge->update(selfcount);
}

Berserker::~Berserker() {

}

void Berserker::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}


void Berserker::jobUpdate() {
	Print << U"Berserker=" << attackdamage;
}

void Berserker::attack() {		//普通の攻撃
	attackdamage = getPower() + getArgs1();
	upcount += 1;
	if (upcount >= 3) {
		selfcount = 5;
	}
	setAttackEvent(attackdamage, EffectType::ULT);
}


void Berserker::BusterAttack() {
	damage(mydamage);
	busterdamage = getPower() * ( getArgs1() / 10);
	if (selfcount > 0) {
		busterdamage *= getArgs2();
		selfcount -= 1;
	}
	if (selfcount == 0) {
		upcount = 0;
	}
	setAttackEvent(busterdamage, EffectType::ULT);
}

void Berserker::upEvent(void) {
	attack();
	chargeGauge->update(selfcount);
}
void Berserker::downEvent(void) {
	BusterAttack();
	chargeGauge->update(selfcount);
}
void Berserker::damageEvent(void) {
	chargeGauge->update(selfcount);
}

