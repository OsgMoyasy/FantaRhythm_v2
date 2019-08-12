#pragma once
#include <math.h>
#include "Observer.h"
#include "FantaRhythm_v2.h"

class Character {
public:
	Character(CSVData &csv , double ix, double iy,int row);
	~Character();
	void chaDraw();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void getEvent(Obj obj)=0;
	void moveUpDown();
	void moveRigthLight();
	void damage(Obj obj);

private:
	int chnumber;	//キャラの番号
	String name;	//キャラの名前
	int hp;			//ヒットポイント
	int power;		//攻撃力ベース
	int args1, args2;//汎用値
	double initx, inity;//基本位置
	double x, y;//現在位置
	static int count;//上下移動に使うフレームカウント
};