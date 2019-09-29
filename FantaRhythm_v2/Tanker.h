#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Tanker :public Character {
public:
	Tanker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Tanker();
private:
	int sealedcount;
	void upAttack();
	void chargeClear();
	void downAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};