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
	void getEvent(Massage msg);
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
	void recovery(int amount);
	void damage(int damage);
protected:
	virtual void upEvent(void) = 0;
	virtual void downEvent(void) = 0;
	virtual void damageEvent(void) = 0;
private:
	class CharacterSubject* csubject;
	class FlipEffect* flipeffect[EffectType::SIZE];// nomarl ult damage 3種類
	int characterNum;	//キャラの番号
	String name;	//キャラの名前
	int hp;			//ヒットポイント
	int power;		//攻撃力ベース
	int args1, args2;//汎用値
	double initx, inity;//基本位置
	double x, y;//現在位置
	int framecnt;//上下移動に使うフレームカウント
	static bool isGuard;

	void guard(void);
};