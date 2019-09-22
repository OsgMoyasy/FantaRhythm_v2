#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Healer.h"


class Sage :public Character, public Healer{
public:
	Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Sage();
	void update();
	void draw();
protected:
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
private:
	int isHeal();
	int recoverycount;
	int curehp;
	void recoverycharge();
	void recoveryClear();

};