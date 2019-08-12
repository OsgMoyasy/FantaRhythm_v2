#pragma once
#include <math.h>
#include "Observer.h"
#include "FantaRhythm_v2.h"

class Character {
public:
	Character(CSVData &csv , double ix, double iy,int row);
	~Character();
	void chaDraw();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void getEvent(Obj obj)=0;
	void moveUpDown();
	void moveRigthLight();
	void damage(Obj obj);

private:
	int chnumber;	//�L�����̔ԍ�
	String name;	//�L�����̖��O
	int hp;			//�q�b�g�|�C���g
	int power;		//�U���̓x�[�X
	int args1, args2;//�ėp�l
	double initx, inity;//��{�ʒu
	double x, y;//���݈ʒu
	static int count;//�㉺�ړ��Ɏg���t���[���J�E���g
};