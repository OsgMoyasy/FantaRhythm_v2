#pragma once
#include <Siv3D.hpp>

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second);//���[�h��ʂ̎��Ԏw��
	bool isUpdate();
	void draw();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;

	void calsAlpha();
};