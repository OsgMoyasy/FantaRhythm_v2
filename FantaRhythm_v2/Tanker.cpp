#include "Tanker.h"
//constexpr int CHARGEMAX = 10;

//防御前提のキャラクター
//上ボタンで攻撃、下ボタンでも攻撃（攻撃ダメージも同じ）
//↑↓↑↓のコンボでガードをはる

Tanker::Tanker(CharacterSubject* csubject, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy) :Character(csubject, U"tanker", char_name, hp, power, generic1, generic2, ix, iy) {
	chargeClear();
}

Tanker::~Tanker() {

}

void Tanker::jobDraw() {
}

void Tanker::jobUpdate() {
	//Print << U"sealed=" << sealedcount;
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
		//onGuardFlag();
	}
	
}

void Tanker::upEvent(void) {
	upAttack();
}
void Tanker::downEvent(void) {
	downAttack();
}
void Tanker::damageEvent(void) {
	chargeClear();
}

