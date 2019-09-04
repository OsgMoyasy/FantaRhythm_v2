#pragma once
#include "FantaRhythm_v2.h"
#include <math.h>


//�����I�ɂ݂�Ȃ�I�u�U�[�o�[�ł��Ƃ�
//�Z���I�Ɏ�������Ȃ�L�����N�^�[�Z�b�g�̉��ɒu��

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
	int totaldmg;//�G�ɗ^�������_���[�W
	const int inity, initx;
	int x, y;
};