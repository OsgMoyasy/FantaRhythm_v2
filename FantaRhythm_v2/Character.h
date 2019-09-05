#pragma once
#include <math.h>
#include "Observer.h"
#include <Siv3D.hpp>
#include "GameEffect.h"

class Character {
public:
	Character(const CSVData &csv , double ix, double iy,int row, CharacterSubject* csubject,const FilePath& effectname);
	~Character();
	void chaDraw();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void getEvent(Massage msg)=0;
	void moveUpDown();
	void moveRigthLight();
	void damage(Obj obj);
	int getPower();
	int getArgs1();
	int getArgs2();
	void setAttackEvent(int attack);
	void playEffect(void);
	void drawEffect(void);

private:
	class CharacterSubject* csubject;
	class FlipEffect* flipeffect;
	int chnumber;	//キャラの番号
	String name;	//キャラの名前
	int hp;			//ヒットポイント
	int power;		//攻撃力ベース
	int args1, args2;//汎用値
	double initx, inity;//基本位置
	double x, y;//現在位置
	int framecnt;//上下移動に使うフレームカウント
	
	
	
};