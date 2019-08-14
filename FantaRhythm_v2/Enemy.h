#pragma once
#include "FantaRhythm_v2.h"
#include <math.h>


//長期的にみるならオブザーバーでやりとり
//短期的に実装するならキャラクターセットの下に置く

class Enemy {
public:
	Enemy(String path);
	~Enemy();
	void update();
	void draw();
	void addDamage(int dmg);
	int getTotalDamage();

private:
	void moveUpDown();
	int cnt;
	int totaldmg;//敵に与えた総ダメージ
	int x, y;
};