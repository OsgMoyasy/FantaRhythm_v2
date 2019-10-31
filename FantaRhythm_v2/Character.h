#pragma once
#include <math.h>
#include "Observer.h"
#include <Siv3D.hpp>
#include "GameEffect.h"

class Character {
public:
	Character(CharacterSubject* csubject, const FilePath& jobname, String& char_name, int hp, int power, double generic1, double generic2, double ix, double iy);
	~Character(void);

	enum EffectType {
		NOMAL,
		ULT,
		DAMAGE,
		GUARD,
		HEAL,
		SIZE,
	};
	bool isReady();
	void update(void);
	void draw(void);
	void getEvent(Massage msg);//CharacterSet����C�x���g���
	int getHp();

	void recovery(int amount);	//�L�����̉�
	void damage(int damage);	//�L�����ւ̃_���[�W
protected:
	//job�Ŏg�p����Q�b�^�[
	String Character::getName();
	int getPower(void);
	double getArgs1(void);
	double getArgs2(void);
	double getX(void);
	double getY(void);
	int    getW(void);
	int    getH(void);
	//�G�t�F�N�g
	void playEffect(EffectType type);
	void playEffect(EffectType type, double x, double y);
	
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
	String char_name;		//�L�����̖��O
	int hp;				//�q�b�g�|�C���g
	int power;			//�U���̓x�[�X
	double generic1, generic2;	//�ėp�l
	double initx, inity;//��{�ʒu
	double x, y;		//���݈ʒu
	int framecnt;		//�㉺�ړ��Ɏg���t���[���J�E���g

	//�L�����ړ�
	void moveUpDown(void);
	void characterDraw(void);
	//�G�t�F�N�g

	void drawEffect(void);
	void guard(void);	//�K�[�h�C�x���g
};