#pragma once
#include <math.h>
#include "Observer.h"
#include <Siv3D.hpp>
#include "GameEffect.h"

class Character {
public:
	Character(CharacterSubject* csubject, const FilePath& jobname, const CSVData& csv, double ix, double iy, int row);
	~Character();

	enum EffectType {
		NOMAL,
		ULT,
		DAMAGE,
		GUARD,
		SIZE,
	};
	
	void draw();
	void update();
	void getEvent(Massage msg);//CharacterSet����C�x���g���
	void moveUpDown();
	void moveRigthLight();
	int getHp();
	int getPower();
	int getArgs1();
	int getArgs2();
	void setAttackEvent(int attack, EffectType type);
	void playEffect(EffectType type);
	void playEffect(EffectType type, double x, double y);
	void drawEffect(void);
	void recovery(int amount);	//�L�����̉�
	void damage(int damage);	//�L�����ւ̃_���[�W
protected:
	//�W���u�N���X�ŃI�[�o�[���C�h
	virtual void jobDraw() = 0;
	virtual void jobUpdate() = 0;
	virtual void upEvent(void) = 0;
	virtual void downEvent(void) = 0;
	virtual void damageEvent(void) = 0;
private:
	class CharacterSubject* csubject;
	class FlipEffect* flipeffect[EffectType::SIZE];// nomarl ult damage 3���
	int characterNum;	//�L�����̔ԍ�
	String name;		//�L�����̖��O
	int hp;				//�q�b�g�|�C���g
	int power;			//�U���̓x�[�X
	int args1, args2;	//�ėp�l
	double initx, inity;//��{�ʒu
	double x, y;		//���݈ʒu
	int framecnt;		//�㉺�ړ��Ɏg���t���[���J�E���g

	void guard(void);	//�K�[�h�C�x���g
	void characterDraw();
};