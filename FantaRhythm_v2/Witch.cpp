#include "Witch.h"
constexpr int CHARGEMAX = 10;

//仕様的にはSoldierと同じ
//魔女のジョブ

Witch::Witch(CharacterSubject* csubject, const CSVData & csv, double ix, double iy, int row) : Character(csubject, U"witch", csv, ix, iy, row) {
	chargeClear();
	chargedamage = 0;
}

Witch::~Witch() {

}

void Witch::draw() {
	Character::characterDraw();
}

void Witch::update() {
	moveUpDown();
	Print << U"charge=" << chargecount;
}

void Witch::charge() {				//Witchは基本チャージだけ
	if (chargecount < CHARGEMAX) {
		chargecount += 1;
	}
}

void Witch::chargeClear() {
	chargecount = 1;
}

void Witch::chargeAttack() {
	chargedamage = (getPower()+getArgs1()) * chargecount ;
	if (chargecount > 8) {						//チャージのカウントが8回を超えた時
		chargedamage = ((getPower() + getArgs1()) * chargecount )* getArgs2();
	}
	setAttackEvent(chargedamage, EffectType::NOMAL);
	chargeClear();
}

void Witch::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		charge();
		break;
	case Massage::DOWNATTACK:
		chargeAttack();
		break;
	case Massage::DAMAGE:
		chargeClear();
		break;
	}
}

