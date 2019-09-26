#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
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