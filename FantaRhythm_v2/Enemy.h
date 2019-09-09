#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Observer.h"
#include "GameEffect.h"
#include "Character.h"


class Enemy :public Observer{
public:
	Enemy(String path);
	~Enemy(void);
	void update(void);
	void draw(void);
	void addDamage(int dmg);
	int getTotalDamage(void);
	void funcEvent(Obj obj);
	void attack(void);
private:
	FlipEffect* flipeffect[EffectType::SIZE];
	int framecnt;
	int totaldmg;//ìGÇ…ó^Ç¶ÇΩëçÉ_ÉÅÅ[ÉW
	const int inity, initx;
	int x, y;

	void moveUpDown(void);
};