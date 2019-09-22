#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Tanker :public Character {
public:
	Tanker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Tanker();
	void update();
	void draw();
protected:
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
private:
	int sealedcount;
	void upAttack();
	void chargeClear();
	void downAttack();
};