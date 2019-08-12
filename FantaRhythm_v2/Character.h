#pragma once
#include <math.h>
#include "Observer.h"
#include "FantaRhythm_v2.h"

class Character {
public:
	Character(CSVData &csv , double ix, double iy,int row);
	~Character();
	void chaDraw();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void getEvent()=0;
	void moveUpDown();
	void moveRigthLight();
	void damage(Obj obj);

private:
	int chnumber;
	String name;
	int hp;
	int power;
	int args1, args2;
	double initx, inity;
	double x, y;
	static int count;
};