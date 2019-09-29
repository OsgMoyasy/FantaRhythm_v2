#pragma once
#include <math.h>
#include "Observer.h"
#include <Siv3D.hpp>
#include "GameEffect.h"

class Character {
public:
	Character(CharacterSubject* csubject, const FilePath& jobname, const CSVData& csv, double ix, double iy, int row);
	~Character(void);

	enum EffectType {
		NOMAL,
		ULT,
		DAMAGE,
		GUARD,
		SIZE,
	};

	void update(void);
	void draw(void);
	void getEvent(Massage msg);//CharacterSet����C�x���g���
	int getHp();

	void recovery(int amount);	//�L�����̉�
	void damage(int damage);	//�L�����ւ̃_���[�W
protected:
	//job�Ŏg�p����Q�b�^�[
	int getPower(void);
	double getArgs1(void);
	double getArgs2(void);
	double getX(void);
	double getY(void);
	int    getW(void);
	int    getH(void);
	String getName(void);
	
	void setAttackEvent(int attack, EffectType type);//�G�ւ̍U���C�x���g
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
	double args1, args2;	//�ėp�l
	double initx, inity;//��{�ʒu
	double x, y;		//���݈ʒu
	int framecnt;		//�㉺�ړ��Ɏg���t���[���J�E���g
	//�L�����ړ�
	void moveUpDown(void);
	void characterDraw(void);
	//�G�t�F�N�g
	void playEffect(EffectType type);
	void playEffect(EffectType type, double x, double y);
	void drawEffect(void);
	void guard(void);	//�K�[�h�C�x���g
};