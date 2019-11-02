#pragma once
#include <Siv3D.hpp>
#include "Scene.h"
#include"MyKey.h"
#include "SceneManager.h"
#include <set>
#include <vector>

constexpr int CHARACTER_SIZE = 4;

class Party :public Scene{
private:
	String msg;
	int msgX;
	int msgY;
	std::vector<int> char_vec;

	bool canCharSet(int character_id);

public:
	Party();
	~Party();
	bool isReady(void);
	void start(void);
	void update(void);
	void draw(void);
};