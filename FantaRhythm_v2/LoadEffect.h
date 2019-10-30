#pragma once
#include <Siv3D.hpp>
#include "Scene.h"
#include "MyKey.h"

class LoadEffect {
public:
	LoadEffect();
	~LoadEffect();
	void setTimer(int second);//���[�h��ʂ̎��Ԏw��
	bool isUpdate(bool isLoad);
	void draw();
	bool isLoadStanby();
private:
	Stopwatch stopwatch;
	double alpha;
	int second;
	
	Scene* prevScene;
	bool dark;//�Ó]���Ȃ�true
	bool setload;//���[�h���Ȃ�true
	bool loadStanby;
	void darkenAlpha();
	void brightenAlpha();
};