#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Soldier :public Character {
public:	
	Soldier(const CSVData &csv, double ix, double iy, int row, CharacterSubject* csubject);
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