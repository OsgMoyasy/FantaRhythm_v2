#pragma once
#include "FantaRhythm_v2.h"
class SceneManager {
public:
	SceneManager();
	void sceneUpdate();
private:
	int nextScene;
	int nowScene;
	void changeScene();

	Title *title;
	SelectMusic *selectMusic;
};
