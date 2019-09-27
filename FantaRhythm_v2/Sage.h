#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Healer.h"
#include "Gauge.h"


class Sage :public Character, public Healer{
public:
	Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Sage();
private:
	int isHeal();
	int recoverycount;
	int curehp;
	void recoverycharge();
	void recoveryClear();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;

	//チャージ計算描画
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//変動するゲージ幅, Ｙ座標
};