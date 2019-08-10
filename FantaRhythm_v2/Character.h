#pragma once
#include <math.h>



class Character {
public:
	Character(double ix, double iy);
	~Character();
	void draw();
	void update();

private:
	int initx, inity;
	double x, y;
	static int count;
};