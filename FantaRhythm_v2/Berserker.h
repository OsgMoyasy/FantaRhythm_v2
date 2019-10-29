#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Berserker :public Character {
public:
	Berserker(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
	~Berserker();
private:
	int mydamage;
	int selfcount;
	int upcount;
	int attackdamage;
	int busterdamage;
	void attack();
	void BusterAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;

	//ƒ`ƒƒ[ƒWŒvZ•`‰æ
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//•Ï“®‚·‚éƒQ[ƒW•, ‚xÀ•W
};