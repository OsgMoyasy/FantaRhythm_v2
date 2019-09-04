#pragma once
#include <Siv3D.hpp>
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
	int cnt;
	int totaldmg;//敵に与えた総ダメージ
	int x, y;
};