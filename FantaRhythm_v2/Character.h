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
	void getEvent(Massage msg);//CharacterSetからイベント受取
	int getHp();

	void recovery(int amount);	//キャラの回復
	void damage(int damage);	//キャラへのダメージ
protected:
	//jobで使用するゲッター
	int getPower(void);
	double getArgs1(void);
	double getArgs2(void);
	double getX(void);
	double getY(void);
	int    getW(void);
	int    getH(void);
	String getName(void);
	
	void setAttackEvent(int attack, EffectType type);//敵への攻撃イベント
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
	double args1, args2;	//汎用値
	double initx, inity;//基本位置
	double x, y;		//現在位置
	int framecnt;		//上下移動に使うフレームカウント
	//キャラ移動
	void moveUpDown(void);
	void characterDraw(void);
	//エフェクト
	void playEffect(EffectType type);
	void playEffect(EffectType type, double x, double y);
	void drawEffect(void);
	void guard(void);	//ガードイベント
};