#pragma once
#include <math.h>
#include "Observer.h"
#include <Siv3D.hpp>
#include "GameEffect.h"

namespace EffectType{
	enum Type {
		NOMAL,
		ULT,
		DAMAGE,
		SIZE,
	};
};

class Character {
public:
	Character(CharacterSubject* csubject, const FilePath& jobname, const CSVData& csv, double ix, double iy, int row);
	~Character();
	void characterDraw();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void getEvent(Massage msg)=0;
	void moveUpDown();
	void moveRigthLight();
	int getHp();
	int getPower();
	int getArgs1();
	int getArgs2();
	void setAttackEvent(int attack, EffectType::Type type);
	void playEffect(EffectType::Type type);
	void playEffect(EffectType::Type type, double x, double y);
	void drawEffect(void);
	int heal();			//�W���u�N���X�ŃI�[�o�[���C�h
	void recovery();	//��
	void onGuardFlag(void);
	void cheakdamage(int damage);
	void damage(int damage);


private:
	class CharacterSubject* csubject;
	class FlipEffect* flipeffect[EffectType::SIZE];// nomarl ult damage 3���
	int characterNum;	//�L�����̔ԍ�
	String name;	//�L�����̖��O
	int hp;			//�q�b�g�|�C���g
	int power;		//�U���̓x�[�X
	int args1, args2;//�ėp�l
	double initx, inity;//��{�ʒu
	double x, y;//���݈ʒu
	int framecnt;//�㉺�ړ��Ɏg���t���[���J�E���g
	static bool isGuard;
	void guard(void);//flag���~�낵�Ėh��

};