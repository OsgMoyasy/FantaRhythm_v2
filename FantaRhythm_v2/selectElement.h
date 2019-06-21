#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"
#include <vector>
#include <stack>

enum SELECTION {
	MUSIC,
	DIFFICULTY
};


class SelectElement :public Scene {
public:
	SelectElement(void);
	~SelectElement(void);
	int update(void);
	void draw(void);
private:
	int switcher;
	int rotation;
	int elementCount;
	std::stack<class SelectScene*> scene;
	std::vector<String> elementName;
};