#include "Archer.h"
constexpr int ARROWSMAX = 5;

//弓兵のジョブ
//上ボタンで弓を蓄える(最大5本)
//下ボタンで確率攻撃

Archer::Archer(CharacterSubject* csubject, const CSVData & csv, double ix, double iy, int row) :Character(csubject, U"archer",csv, ix, iy, row ) {
	arrowscount = 0;
	arrowsdamage = 0;
}

Archer::~Archer() {

}

void Archer::jobDraw() {

}

void Archer::jobUpdate() {
	Print << U"Having arrows=" << arrowscount;			//持っている矢の数、名前は仮
}

void Archer::arrowscharge() {		//矢のチャージ(5本まで)
	if (arrowscount < ARROWSMAX) {
		arrowscount += 1;
	}
}

void Archer::arrowsClear() {		//攻撃した後、矢が正の整数ならば減らす
	if (arrowscount > 0) {
		arrowscount -= 1;
	}
}

void Archer::arrowsAttack() {
	if (arrowscount > 0) {			//矢の数があればランダムでダメージ、なければ常に1ダメージ
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
