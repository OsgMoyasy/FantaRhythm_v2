#pragma once
#include "FantaRhythm_v2.h"
#include "Observer.h"

class Enemy {
public:
	void update();
	void draw();
	void addDamage(int dmg);
	int getTotalDamage();

private:
	int cnt;
	int totaldmg;
};