#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Observer.h"
#include "GameEffect.h"

class Enemy :public Observer{
public:
	Enemy(String path);
	~Enemy(void);
	void update(void);
	void draw(void);
	void addDamage(int dmg);
	int getTotalDamage(void);
	void funcEvent(Obj obj);

private:
	FlipEffect* flipeffect;
	int framecnt;
	int totaldmg;//�G�ɗ^�������_���[�W
	const int inity, initx;
	int x, y;

	void moveUpDown(void);
};