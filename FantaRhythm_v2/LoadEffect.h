#pragma once
#include <Siv3D.hpp>
#include "Scene.h"
#include "MyKey.h"

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second, Scene* prevScene);//ロード画面の時間指定
	bool isUpdate();
	void draw();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;
	Scene* prevScene;
	
	void calcAlpha();
};