#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Soldier();
private:
	int chargecount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};