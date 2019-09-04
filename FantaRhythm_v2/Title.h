#pragma once
#include "FantaRhythm_v2.h"
#include "Scene.h"
class Title : public Scene{
public:
	Title(void);
	~Title(void);
	void update(void);
	void draw(void);
private:
	double alpha;
	void changeAlpha(void);
};
