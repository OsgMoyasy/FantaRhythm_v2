#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Archer :public Character {
public:
	Archer(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Archer();
	void update();
	void draw();
protected:
	void upEvent(void);
	void downEvent(void);
	void damageEvent(void);
private:
	int arrowscount;
	int arrowsdamage;
	void arrowscharge();
	void arrowsClear();
	void arrowsAttack();
};