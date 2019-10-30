#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include <vector>

class Archer :public Character {
public:
	Archer(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
	~Archer();
private:
	class ArrowEffect* arrowEffect;
	int arrowscount;
	int arrowsdamage;
	void arrowscharge();
	void arrowsClear();
	void arrowsAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};

class ArrowEffect {//�|��̃G�t�F�N�g
public:
	ArrowEffect(double x, double y);
	~ArrowEffect();
	void add();		//�ǉ�
	void take();	//���o��
	void draw(double x, double y);
private:
	double x, y;//��`��ʒu����
	typedef struct _Arrow {
		double offsetX, offsetY;
	}Arrow;
	std::vector<Arrow> arrows;
};