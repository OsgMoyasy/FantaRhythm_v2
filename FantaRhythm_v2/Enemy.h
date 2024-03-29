#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Observer.h"
#include "GameEffect.h"


class Enemy :public Observer{
public:
	Enemy(String path);
	~Enemy(void);

	enum EffectType {
		NOMAL,
		ULT,
		DAMAGE,
		SIZE,
	};

	void update(void);
	void draw(void);
	void addDamage(int dmg);
	int getTotalDamage(void);
	void funcEvent(Obj obj);
	void attack(void);
private:
	FlipEffect* flipeffect[EffectType::SIZE];
	int framecnt;
	int totalDamage;//�G�ɗ^�������_���[�W
	const int inity, initx;
	int x, y;

	void moveUpDown(void);
};