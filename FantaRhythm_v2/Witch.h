#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Witch :public Character {
public:
	Witch(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Witch();
	void update();
	void draw();
	void getEvent(Massage msg);
protected:
	void upEvent(void);
	void downEvent(void);
	void damageEvent(void);
private:
	int chargecount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();
};