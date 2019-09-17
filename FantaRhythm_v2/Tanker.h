#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Tanker :public Character {
public:
	Tanker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Tanker();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int sealedcount;
	void upAttack();
	void chargeClear();
	void downAttack();
};