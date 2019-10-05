#pragma once
#include <Siv3D.hpp>

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second);//ƒ[ƒh‰æ–Ê‚ÌŠÔw’è
	bool isUpdate();
	void draw();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;

	void calsAlpha();
};