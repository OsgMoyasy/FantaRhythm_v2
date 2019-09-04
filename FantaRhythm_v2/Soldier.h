#pragma once
#include "FantaRhythm_v2.h"
#include "Character.h"

class Soldier :public Character {
public:	
	Soldier(CSVData &csv, double ix, double iy, int row);
	~Soldier();
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