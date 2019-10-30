#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Witch :public Character {
public:
	Witch(CharacterSubject* csubject, String& char_name, int hp, int attack, double generic1, double generic2, double ix, double iy);
	~Witch();
private:
	int chargeCount;
	int chargedamage;
	void charge();
	void chargeClear();
	void chargeAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;

	//�`���[�W�v�Z�`��
	class Gauge* chargeGauge;
	double currentChargeW, chaY;	//�ϓ�����Q�[�W��, �x���W
};