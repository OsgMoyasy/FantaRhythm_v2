#include "Tanker.h"
//constexpr int CHARGEMAX = 10;

//防御前提のキャラクター
//上ボタンで攻撃、下ボタンでも攻撃（攻撃ダメージも同じ）
//↑↓↑↓のコンボでガードをはる

Tanker::Tanker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row) :Character(csubject, U"tanker", csv, ix, iy, row) {
	chargeClear();
}

Tanker::~Tanker() {

}

void Tanker::draw() {
	Character::chaDraw();
	drawEffect();
}

void Tanker::update() {
	moveUpDown();
	Print << U"sealed=" << sealedcount;
}

void Tanker::upAttack() {		//小ダメージ　＆　シールドチャージ
	setAttackEvent(getPower(), EffectType::NOMAL);
	if (sealedcount == 0 || sealedcount == 2) {		//奇数分のコンボ加算
		sealedcount += 1;
	}

}

void Tanker::chargeClear() {
	sealedcount = 0;
}

void Tanker::downAttack() {
	setAttackEvent(getPower(), EffectType::ULT);
	if (sealedcount == 1 || sealedcount == 3) {		//偶数分のコンボ加算
		sealedcount += 1;
	}
	if (sealedcount == 4) {
		onGuardFlag();
	}
	
}

void Tanker::getEvent(Massage msg) {
	switch (msg) {
	case Massage::UPATTACK:
		upAttack();
		break;
	case Massage::DOWNATTACK:
		downAttack();
		break;
	case Massage::DAMAGE:
		chargeClear();
		break;
	}
}

