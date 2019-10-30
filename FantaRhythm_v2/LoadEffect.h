#pragma once
#include <Siv3D.hpp>
#include "Scene.h"
#include "MyKey.h"

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second);//ロード画面の時間指定
	bool isUpdate(bool isLoad);
	void draw();
	bool isLoadStanby();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;
	
	Scene* prevScene;
	bool dark;//暗転中ならtrue
	bool setload;//ロード中ならtrue
	bool loadStanby;
	void darkenAlpha();
	void brightenAlpha();
};