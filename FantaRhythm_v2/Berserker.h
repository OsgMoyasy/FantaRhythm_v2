#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Berserker :public Character {
public:
	Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Berserker();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int mydamage;
	int attackdamage;
	int busterdamage;
	void attack();
	void BusterAttack();
};