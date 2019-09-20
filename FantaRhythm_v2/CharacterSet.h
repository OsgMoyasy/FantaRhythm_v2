#pragma once
#include <Siv3D.hpp>
#include"Character.h"
#include "Observer.h"
#include "Enemy.h"

#define CHANUMBER 4



class CharacterSet:public Observer{
public:
	CharacterSet(int save[], const String& musicpath);
	~CharacterSet();
	void draw();
	void update();
	void funcEvent(Obj obj);
	void TotalhpDraw();
	int getTotalDamage(void);
	int getCurrentHp(void);
	void gameEndEffect(void);
private:
	int totalhp;
	int hpx;
	int damage;
	class Character* cha[CHANUMBER];
	class Enemy* enemy;
	class CharacterSubject* csubject;//�L�����N�^�[�X����C�x���g���󂯎�邽�߂�
	void damageToSelves(int lane);

	void calchpx(int currenthp);
};