#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Berserker :public Character {
public:
	Berserker(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Berserker();
private:
	int mydamage;
	int selfcount;
	int attackdamage;
	int busterdamage;
	void attack();
	void BusterAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};