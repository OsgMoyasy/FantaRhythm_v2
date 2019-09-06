#pragma once
#include "FantaRhythm_v2.h"
#include "Character.h"

class Witch :public Character {
public:
	Witch(CSVData& csv, double ix, double iy, int row, CharacterSubject* csubject);
	~Witch();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int chargecount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();
};