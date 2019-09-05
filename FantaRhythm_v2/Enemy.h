#pragma once
#include <Siv3D.hpp>
#include <math.h>
#include "Observer.h"


//�����I�ɂ݂�Ȃ�I�u�U�[�o�[�ł��Ƃ�
//�Z���I�Ɏ�������Ȃ�L�����N�^�[�Z�b�g�̉��ɒu��

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
	int totaldmg;//�G�ɗ^�������_���[�W
	const int inity, initx;
	int x, y;
};