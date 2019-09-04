#pragma once
#include "FantaRhythm_v2.h"
#include <math.h>


//長期的にみるならオブザーバーでやりとり
//短期的に実装するならキャラクターセットの下に置く

class Enemy {
public:
	Enemy(String path);
	~Enemy(void);
	void update(void);
	void draw(void);
	void addDamage(int dmg);
	int getTotalDamage(void);

private:
	void moveUpDown(void);
	int framecnt;
	int totaldmg;//敵に与えた総ダメージ
	const int inity, initx;
	int x, y;
};