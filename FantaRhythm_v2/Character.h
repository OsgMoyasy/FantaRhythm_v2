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
	void getEvent(Massage msg);//CharacterSetからイベント受取
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
	void recovery(int amount);	//キャラの回復
	void damage(int damage);	//キャラへのダメージ
protected:
	//ジョブクラスでオーバーライド
	virtual void jobDraw() = 0;
	virtual void jobUpdate() = 0;
	virtual void upEvent(void) = 0;
	virtual void downEvent(void) = 0;
	virtual void damageEvent(void) = 0;
private:
	class CharacterSubject* csubject;
	class FlipEffect* flipeffect[EffectType::SIZE];// nomarl ult damage 3種類
	int characterNum;	//キャラの番号
	String name;		//キャラの名前
	int hp;				//ヒットポイント
	int power;			//攻撃力ベース
	int args1, args2;	//汎用値
	double initx, inity;//基本位置
	double x, y;		//現在位置
	int framecnt;		//上下移動に使うフレームカウント

	void guard(void);	//ガードイベント
	void characterDraw();
};