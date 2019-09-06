#pragma once
#include "FantaRhythm_v2.h"
#include "Character.h"

class Archer :public Character {
public:
	Archer(CSVData& csv, double ix, double iy, int row, CharacterSubject* csubject);
	~Archer();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int arrowscount;
	int arrowsdamage;
	void arrowscharge();
	void arrowsClear();
	void arrowsAttack();
};