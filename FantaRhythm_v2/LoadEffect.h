#pragma once
#include <Siv3D.hpp>

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second);//ロード画面の時間指定
	bool isUpdate();
	void draw();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;

	void calsAlpha();
};