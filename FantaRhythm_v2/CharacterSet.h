#pragma once
#include <Siv3D.hpp>
#include"Character.h"
#include "Observer.h"
#include "Enemy.h"
#include "Gauge.h"

#define CHANUMBER 4

class CharacterSet:public Observer{
public:
	CharacterSet(const String& musicpath);
	~CharacterSet();
	bool isReady();
	void draw();
	void update();
	void funcEvent(Obj obj);
	int getTotalDamage(void);
	int getCurrentHp(void);
	void gameEndEffect(void);
private:
	int totalhp;
	int damage;	//�G�L�����̃_���[�W�@��Փx���ŕύX���₷���悤�ɂƂ肠���������ɒu���Ă���
	class Character* cha[CHANUMBER];
	class Enemy* enemy;
	class CharacterSubject* csubject;//�L�����N�^�[�X����C�x���g���󂯎�邽�߂�
	class Gauge* hpGauge;
	void damageToSelves(int lane, int damage);
};