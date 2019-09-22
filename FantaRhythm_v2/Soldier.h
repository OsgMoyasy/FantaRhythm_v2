#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Soldier();
	void update();
	void draw();
protected:
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
private:
	int chargecount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();
};