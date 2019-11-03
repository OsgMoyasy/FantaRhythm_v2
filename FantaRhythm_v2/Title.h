#pragma once
#include <Siv3D.hpp>
#include "Scene.h"
#include "MyKey.h"

class Title : public Scene{
public:
	Title(void);
	~Title(void);
	bool isReady(void);
	void start(void);
	void update(void);
	void draw(void);
	void drawTutorial(void);
	bool tutorialF;
private:
	Audio *TitleAudio;
	double alpha;
	void changeAlpha(void);
	Texture tutorial_img;
};
