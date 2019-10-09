#pragma once
#include "Judge.h"
#include <Siv3D.hpp>

class JudgeEffect {
public:
	JudgeEffect(FilePath path);	//ジャッジ時エフェクトのファイルパス
	~JudgeEffect();
	void setEffect(JUDGE::TYPE type);
	void update();
	void draw();
private:
	Texture texture[JUDGE::TYPE_SIZE];
	double alpha;
	const double drawSecond = 0.5;//描画時間
	const double drawFixedSecond = 0.3;//描画時の透過率が1になるまでの時間
	Stopwatch stopwatch;
	JUDGE::TYPE type;
	

};