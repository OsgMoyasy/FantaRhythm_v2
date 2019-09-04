#pragma once
#include <Siv3D.hpp>
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
	int cnt;
	int totaldmg;//�G�ɗ^�������_���[�W
	int x, y;
};