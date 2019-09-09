#pragma once
#include <Siv3D.hpp>
#include "Character.h"


class Sage :public Character {
public:
	Sage(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Sage();
	void update();
	void draw();
	void getEvent(Massage msg);

private:
	int recoverycount;
	int curehp;
	void recoverycharge();
	void Qrecovery();

};