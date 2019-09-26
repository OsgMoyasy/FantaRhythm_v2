#pragma once
#include <Siv3D.hpp>
#include "Character.h"
#include <vector>

class Archer :public Character {
public:
	Archer(CharacterSubject* csubject, const CSVData& csv, double ix, double iy, int row);
	~Archer();
private:
	class ArrowEffect* arrowEffect;
	int arrowscount;
	int arrowsdamage;
	void arrowscharge();
	void arrowsClear();
	void arrowsAttack();

	void jobUpdate() final;
	void jobDraw() final;
	void upEvent(void) final;
	void downEvent(void) final;
	void damageEvent(void) final;
};

class ArrowEffect {//弓矢のエフェクト
public:
	ArrowEffect(double x, double y);
	~ArrowEffect();
	void add();		//追加
	void take();	//取り出し
	void draw(double x, double y);
private:
	double x, y;//矢描画位置中央
	typedef struct _Arrow {
		double offsetX, offsetY;
	}Arrow;
	std::vector<Arrow> arrows;
};