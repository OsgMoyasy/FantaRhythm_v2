#pragma once
#include <Siv3D.hpp>
#include "Character.h"


class Sage :public Character {
public:
	Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Sage();
	void update();
	void draw();
protected:
	void upEvent(void);
	void downEvent(void);
	void damageEvent(void);
private:
	int heal();
	int recoverycount;
	int curehp;
	void recoverycharge();
	void recoveryClear();

};