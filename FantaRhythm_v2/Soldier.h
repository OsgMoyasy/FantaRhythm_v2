#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
	~Soldier();
private:

	//�I�[�o�[���C�h
	void jobUpdate(void) final;
	void jobDraw(void) final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
	//�`���[�W�C�x���g
	int chargeCount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();
	//�`���[�W�v�Z�`��
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//�ϓ�����Q�[�W��, �x���W
};