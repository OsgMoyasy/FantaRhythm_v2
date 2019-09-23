#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Archer :public Character {
public:
	Archer(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Archer();
private:
	int arrowscount;
	int arrowsdamage;
	void arrowscharge();
	void arrowsClear();
	void arrowsAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};