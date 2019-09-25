#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Combostar :public Character {
public:
	Combostar(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Combostar();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int updamage;
	int combocount;
	int combodamage;
	void combocharge();
	void comboClear();
	void comboAttack();
};