#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
	~Soldier();
private:

	//オーバーライド
	void jobUpdate(void) final;
	void jobDraw(void) final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
	//チャージイベント
	int chargeCount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();
	//チャージ計算描画
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//変動するゲージ幅, Ｙ座標
};