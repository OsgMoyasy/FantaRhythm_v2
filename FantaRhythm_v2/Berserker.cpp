#include "Berserker.h"
constexpr int CHARGEMAX = 5;

Berserker::Berserker(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) :Character(csubject, U"berserker", char_name, hp, power, generic1, generic2, ix, iy) {
	attackdamage = 0;		//‚±‚Ì“ñ‚Â‚Í‚½‚Ô‚ñŽg‚í‚È‚¢
	busterdamage = 0;	
	upcount = 0;
	selfcount = 0;			//downattack‚Ì‰ñ”
	mydamage = 4;			//‰ºƒ{ƒ^ƒ“UŒ‚‚ÅH‚ç‚¤damage—Ê
	chargeGauge = new Gauge(getX() - getW() / 2.0, getY() + getH() / 2.0, U"resources/images/effects/berserker", CHARGEMAX, Color(Palette::Black), Color(Palette::Darkred));
	chargeGauge->update(selfcount);
}

Berserker::~Berserker() {

}

void Berserker::jobDraw() {
	chargeGauge->draw(getY() + getH() / 2.0);
}


void Berserker::jobUpdate() {
	//Print << U"Berserker=" << attackdamage;
}

void Berserker::attack() {		//•’Ê‚ÌUŒ‚
	attackdamage = getPower() + getArgs1();
	upcount += 1;
	if (upcount >= 3 && selfcount == 0) {
		selfcount = 5;
	}
	setAttackEvent(attackdamage, EffectType::NOMAL);
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
	setAttackEvent(busterdamage, EffectType::NOMAL);
	playEffect(EffectType::ULT, getX(), getY() + 40);
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

