#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Observer.h"


//長期的にみるならオブザーバーでやりとり
//短期的に実装するならキャラクターセットの下に置く

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
	void moveUpDown(void);
	int framecnt;
	int totaldmg;//敵に与えた総ダメージ
	const int inity, initx;
	int x, y;
};