#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Berserker :public Character {
public:
	Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Berserker();
	void update();
	void draw();
protected:
	void upEvent(void);
	void downEvent(void);
	void damageEvent(void);

private:
	int mydamage;
	int attackdamage;
	int busterdamage;
	void attack();
	void BusterAttack();
};