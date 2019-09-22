#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Witch :public Character {
public:
	Witch(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Witch();
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