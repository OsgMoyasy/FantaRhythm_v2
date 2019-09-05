#pragma once
#include <Siv3D.hpp>
#include"Character.h"
#include "Observer.h"
#include "Enemy.h"

#define CHANUMBER 4
namespace JOB {
	enum LIST {
		SOLDIER,

	};
}


class CharacterSet:public Observer{
public:
	CharacterSet(int save[], const String& musicpath);
	~CharacterSet();
	void draw();
	void update();
	void funcEvent(Obj obj);
	void TotalhpDraw();
	int getTotalDamage(void);
private:
	int totalhp;
	int starthp;	//ダメージを受けた時に増やす値
	int fixedhp;
	int damage;     //仮のダメージ
	class Character* cha[CHANUMBER];
	class Enemy* enemy;
	

};