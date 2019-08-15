#pragma once
#include "FantaRhythm_v2.h"
#include <math.h>


//�����I�ɂ݂�Ȃ�I�u�U�[�o�[�ł��Ƃ�
//�Z���I�Ɏ�������Ȃ�L�����N�^�[�Z�b�g�̉��ɒu��

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
	int totaldmg;//�G�ɗ^�������_���[�W
	int x, y;
};