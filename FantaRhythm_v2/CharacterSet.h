#pragma once
#include <Siv3D.hpp>
#include"Character.h"
#include "Observer.h"
#include "Enemy.h"

#define CHANUMBER 4
namespace JOB {
	enum LIST {
		SOLDIER,
		ARCHER,
		WITCH,
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
	void selfDamage(void);
private:
	int totalhp;
	int currenthp;
	int hpx;
	int damage;
	class Character* cha[CHANUMBER];
	class Enemy* enemy;
	class CharacterSubject* csubject;//キャラクター個々からイベントを受け取るために

	void calchpx(void);
};