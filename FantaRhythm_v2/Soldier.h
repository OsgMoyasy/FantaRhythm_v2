#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include "Gauge.h"

class Soldier :public Character {
public:	
	Soldier(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
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