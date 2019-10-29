#pragma once
#include <Siv3D.hpp>
#include "Character.h"

class Tanker :public Character {
public:
	Tanker(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
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