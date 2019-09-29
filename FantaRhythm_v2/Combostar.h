#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Combostar :public Character {
public:
	Combostar(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Combostar();
private:
	int updamage;
	int combocount;
	int combodamage;
	void combocharge();
	void comboClear();
	void comboAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;

	//チャージ計算描画
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//変動するゲージ幅, Ｙ座標
};